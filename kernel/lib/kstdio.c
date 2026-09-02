#include "kernel/lib/kstdio.h"
#include <stdint.h>
#include <stdbool.h>
#include "drivers/screen/screen.h"
#include "kernel/lib/karith64.h"

size_t _format_num( char* buf, size_t size, va_list *argsp, format_length_state_t len, bool sign, int radix, int width, bool zero_pad);

/*
    A simplified printf implementation that calls a separate formatter function
    and prints to the screen. 
*/
void printf( const char* fmt, ... ) {
    va_list args;
    va_start(args, fmt);

    char buf[256] = {0};
    kvformat(buf, 256, fmt, args);

    print((uint8_t*)buf);
    va_end(args);
}

const char hexChars[] = "0123456789abcdef";
size_t _format_num( char* buf, size_t size, va_list *argsp, format_length_state_t len, bool sign, int radix, int width, bool zero_pad ) {
    unsigned long long number = 0;
    int num_sign = 1;

    switch (len)
    {
    case FORMAT_LENGTH_SHORT_SHORT:
    case FORMAT_LENGTH_SHORT:
    case FORMAT_LENGTH_DEFAULT:
        if(sign) {
            int n = va_arg(*argsp, int);
            if(n < 0) {
                num_sign = -1;
                number = (unsigned long long)(-(n+1))+1;
            }
            else number = (unsigned long long)n;
        }
        else
            number = va_arg(*argsp, unsigned int);
        break;
    case FORMAT_LENGTH_LONG:
        if(sign) {
            long n = va_arg(*argsp, long);
            if(n < 0) {
                num_sign = -1;
                number = (unsigned long long)(-(n+1))+1;
            }
            else number = (unsigned long long)n;
        }
        else
            number = va_arg(*argsp, unsigned long);

        break;
    case FORMAT_LENGTH_LONG_LONG:
        if(sign) {
            long long n = va_arg(*argsp, long long);
            if(n < 0) {
                num_sign = -1;
                number = (unsigned long long)(-(n+1))+1;
            }
            else number = (unsigned long long)n;
        }
        else
            number = va_arg(*argsp, unsigned long long);

        break;
    
    default: break;
    }

    char buffer [65] = {0};
    int _pos = 0;
    do
    {
        uint64_t rem = 0;
        div64by32(number, (uint32_t)radix, &number, &rem);
        buffer[_pos++] = hexChars[rem];
    } while (number > 0);

    int total_len = _pos + (sign && (num_sign == -1));
    int pad = (zero_pad && width > total_len) ? (width - total_len) : 0;

    size_t out = 0;
    if(sign && num_sign == -1 && out < size) buf[out++] = '-';
    for(int i = 0; i < pad && out < size; ++i) buf[out++] = '0';
    while (--_pos >= 0 && out < size - 1) buf[out++] = buffer[_pos];

    return out;
}

/*
    A formatter function that works on a given format string
    and returns the number of characters written. This is a simple implementation
    not consisting of all the flags and specifiers.
*/
size_t kvformat(char* buf, size_t size, const char* fmt, va_list args) {
    if(!buf || !fmt || size <= 0) return 0;

    format_state_t STATE = FORMAT_NORMAL;
    format_length_state_t LENGTH = FORMAT_LENGTH_DEFAULT;
    size_t pos = 0;
    int width = 0;
    bool zero_pad = false;

    while (*fmt)
    {
        if(pos >= size - 1) break;
        switch (STATE)
        {
        case FORMAT_NORMAL:
            if(*fmt == '%') {
                LENGTH = FORMAT_LENGTH_DEFAULT;
                zero_pad = false;
                width = 0;
                STATE = FORMAT_WIDTH;
            }
            else buf[pos++] = (char)*fmt;
            break;
        
        case FORMAT_WIDTH:
            if(*fmt == '0' && width == 0) zero_pad = true;
            else if(*fmt >= '0' && *fmt <= '9') width = width*10 + (*fmt - '0');
            else {
                STATE = FORMAT_LENGTH;
                --fmt;
            }
            break;

        case FORMAT_LENGTH:
            switch (*fmt)
            {
            case 'l':
                LENGTH = FORMAT_LENGTH_LONG;
                STATE = FORMAT_LONG;
                break;
            case 'h':
                LENGTH = FORMAT_LENGTH_SHORT;
                STATE = FORMAT_SHORT;
                break;
            default:
                STATE = FORMAT_SPECIFIER;
                --fmt;
                break;
            }
            break;

        case FORMAT_LONG:
            if(*fmt == 'l') {
                LENGTH = FORMAT_LENGTH_LONG_LONG;
                STATE = FORMAT_SPECIFIER;
            }
            else {
                STATE = FORMAT_SPECIFIER;
                --fmt;
            }
            break;
        
        case FORMAT_SHORT:
            if(*fmt == 'h') {
                LENGTH = FORMAT_LENGTH_SHORT_SHORT;
                STATE = FORMAT_SPECIFIER;
            }
            else {
                STATE = FORMAT_SPECIFIER;
                --fmt;
            }
            break;
        
        case FORMAT_SPECIFIER:
            switch (*fmt)
            {
            case 'c': {
                int c = va_arg(args, int);
                if(pos < size - 1)
                    buf[pos++] = (char)c;
                STATE = FORMAT_NORMAL;
                break;
            }
            case 's': {
                char* str = va_arg(args, char*);
                while(*str && pos < size - 1) buf[pos++] = *str++;
                STATE = FORMAT_NORMAL;
                break;
            }
            case '%':
                if(pos < size - 1) buf[pos++] = '%';
                STATE = FORMAT_NORMAL;
                break;
            case 'd':
            case 'i':
                pos += _format_num(buf+pos, size-pos, &args, LENGTH, true, 10, width, zero_pad);
                STATE = FORMAT_NORMAL;
                break;
            case 'u':
                pos += _format_num(buf+pos, size-pos, &args, LENGTH, false, 10, width, zero_pad);
                STATE = FORMAT_NORMAL;
                break;
            case 'o':
                pos += _format_num(buf+pos, size-pos, &args, LENGTH, false, 8, width, zero_pad);
                STATE = FORMAT_NORMAL;
                break;
            case 'X':
            case 'x':
                pos += _format_num(buf+pos, size-pos, &args, LENGTH, false, 16, width, zero_pad);
                STATE = FORMAT_NORMAL;
                break;
            default: 
                if(pos < size - 1) buf[pos++] = '%';
                if(pos < size - 1) buf[pos++] = *fmt;
                STATE = FORMAT_NORMAL;
                break;
            }
            break;

        default:
            break;
        }

        ++fmt;
    }
    buf[pos] = 0;
    return pos;
}