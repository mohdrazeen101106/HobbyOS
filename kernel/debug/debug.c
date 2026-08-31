#include "debug.h"
#include <stdint.h>

#include "../../drivers/screen/screen.h"
#include "../lib/karith64.h"

void debug_trigger_invalid_opcode(void)
{
    __asm__ volatile ("ud2");
}

void test_karith64_div64by32(void)
{
    uint64_t dividend = 96;
    uint32_t divisor = 5;
    uint64_t quotient = 0;
    uint64_t remainder = 0;

    div64by32(dividend, divisor, &quotient, &remainder);
    print_char((uint8_t)(remainder+48), -1, -1, WHITE_ON_BLACK);
}