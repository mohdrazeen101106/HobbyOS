#include "kernel/lib/karith64.h"

void div64by32(uint64_t dividend, uint32_t divisor, uint64_t* quotient, uint64_t* remainder) {
    *remainder = dividend >> 32;
    *quotient = 0;
    uint8_t mask_index = 32;
    while (mask_index < 64) {
        uint32_t bit_pos = 63 - mask_index;
        *remainder = (*remainder << 1) | ((dividend >> bit_pos) & 1ULL);
        *quotient <<= 1;
        if (*remainder >= divisor) {
            *remainder -= divisor;
            *quotient |= 1ULL;
        }
        ++mask_index;
    }
}