#ifndef KARITH64_H
#define KARITH64_H

#include <stdint.h>

void div64by32(uint64_t dividend, uint32_t divisor, uint64_t* quotient, uint64_t* remainder);

#endif