#ifndef EXCEPTION_STATS_H
#define EXCEPTION_STATS_H

#include <stdint.h>

#define EXCEPTION_VECTOR_COUNT 32

void exception_stats_record(uint32_t vector);

uint32_t exception_stats_get(uint32_t vector);

#endif