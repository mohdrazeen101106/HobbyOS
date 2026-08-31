#include "kstdio.h"
#include <stdint.h>
#include <stdbool.h>
#include "../../drivers/screen/screen.h"
#include "../lib/karith64.h"

int* printf_num( int* argp, printf_length_state_t len, bool sign, int radix);

/*
    My simplified implementation of the printf function from the C standard library
    Currently only prints to the VGA buffer using the screen driver, later plans are to print
    to STDOUT after that itself is implemented!
*/
void printf( const char* fmt, ... ) {
    printf_state_t STATE = PRINTF_NORMAL;
    printf_length_state_t LENGTH = PRINTF_LENGTH_DEFAULT;

    int* argp = (int*) &fmt;
    argp += sizeof(fmt) / sizeof(int);

    while (*fmt)
    {
        switch (STATE)
        {
        case PRINTF_NORMAL:
            if(*fmt == '%') {
                LENGTH = PRINTF_LENGTH_DEFAULT;
                STATE = PRINTF_LENGTH;
            }
            else print_char((uint8_t)*fmt, -1, -1, WHITE_ON_BLACK); // Print the character to screen
            break;
        
        case PRINTF_LENGTH:
            switch (*fmt)
            {
            case 'l':
                LENGTH = PRINTF_LENGTH_LONG;
                STATE = PRINTF_LONG;
                break;
            case 'h':
                LENGTH = PRINTF_LENGTH_SHORT;
                STATE = PRINTF_SHORT;
                break;
            default:
                STATE = PRINTF_SPECIFIER;
                --fmt;
                break;
            }
            break;

        case PRINTF_LONG:
            if(*fmt == 'l') {
                LENGTH = PRINTF_LENGTH_LONG_LONG;
                STATE = PRINTF_SPECIFIER;
            }
            else {
                STATE = PRINTF_SPECIFIER;
                --fmt;
            }
            break;
        
        case PRINTF_SHORT:
            if(*fmt == 'h') {
                LENGTH = PRINTF_LENGTH_SHORT_SHORT;
                STATE = PRINTF_SPECIFIER;
            }
            else {
                STATE = PRINTF_SPECIFIER;
                --fmt;
            }
            break;
        
        case PRINTF_SPECIFIER:
            switch (*fmt)
            {
            case 'c':
                print_char((uint8_t)*argp, -1, -1, WHITE_ON_BLACK);
                ++argp;
                STATE = PRINTF_NORMAL;
                break;
            case 's':
                print(*(uint8_t**)argp);
                ++argp;
                STATE = PRINTF_NORMAL;
                break;
            case '%':
                print_char((uint8_t)'%', -1, -1, WHITE_ON_BLACK);
                STATE = PRINTF_NORMAL;
                break;
            case 'd':
            case 'i':
                argp = printf_num(argp, LENGTH, true, 10);
                STATE = PRINTF_NORMAL;
                break;
            case 'u':
                argp = printf_num(argp, LENGTH, false, 10);
                STATE = PRINTF_NORMAL;
                break;
            case 'o':
                argp = printf_num(argp, LENGTH, false, 8);
                STATE = PRINTF_NORMAL;
                break;
            case 'X':
            case 'x':
                argp = printf_num(argp, LENGTH, false, 16);
                STATE = PRINTF_NORMAL;
                break;
            default: break;
            }
            break;

        default:
            break;
        }

        ++fmt;
    }
    
}

const uint8_t hexChars[] = "0123456789abcdef";
int* printf_num( int* argp, printf_length_state_t len, bool sign, int radix ) {
    unsigned long long number = 0;
    int num_sign = 1;

    switch (len)
    {
    case PRINTF_LENGTH_SHORT_SHORT:
    case PRINTF_LENGTH_SHORT:
    case PRINTF_LENGTH_DEFAULT:
        if(sign) {
            int n = *argp;
            if(n < 0) {
                n = -n;
                num_sign = -1;
            }
            number = (unsigned long long)n;
        }
        else
            number = *(unsigned int*)argp;
        
        ++argp;
        break;
    case PRINTF_LENGTH_LONG:
        if(sign) {
            long n = *(long*)argp;
            if(n < 0) {
                n = -n;
                num_sign = -1;
            }
            number = (unsigned long long)n;
        }
        else
            number = *(unsigned long*)argp;

        ++argp;
        break;
    case PRINTF_LENGTH_LONG_LONG:
        if(sign) {
            long long n = *(long long*)argp;
            if(n < 0) {
                n = -n;
                num_sign = -1;
            }
            number = (unsigned long long)n;
        }
        else
            number = *(unsigned long long*)argp;

        argp += 2;
        break;
    
    default: break;
    }

    uint8_t buffer [32] = {0};
    int pos = 0;
    do
    {
        uint64_t rem = 0;
        div64by32(number, (uint32_t)radix, &number, &rem);
        buffer[pos++] = hexChars[rem];
    } while (number > 0);

    if(sign && num_sign == -1) buffer[pos++] = (uint8_t)'-';
    while (--pos >= 0) print_char(buffer[pos], -1, -1, WHITE_ON_BLACK);
    
    return argp;
}