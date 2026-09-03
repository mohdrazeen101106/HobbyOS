#include "kernel/exceptions/stats.h"

static uint32_t exception_counts[EXCEPTION_VECTOR_COUNT] = {0};

void exception_stats_record(uint32_t vector) {
    if (vector >= EXCEPTION_VECTOR_COUNT) return;
    ++exception_counts[vector];
}

uint32_t exception_stats_get(uint32_t vector) {
    if (vector >= EXCEPTION_VECTOR_COUNT) return 0;
    return exception_counts[vector];
}