#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    EXCEPTION_FAULT,
    EXCEPTION_TRAP,
    EXCEPTION_ABORT
} exception_type_t;

typedef struct {
    uint8_t vector;
    const char* name;
    const char* description;
    exception_type_t type;
    bool has_error_code;
    bool recoverable;
} exception_descriptor_t;

const exception_descriptor_t* exception_get_descriptor(uint32_t vector);
const char* exception_get_name(uint32_t vector);
const char* exception_get_description(uint32_t vector);
bool exception_has_error_code(uint32_t vector);

#endif