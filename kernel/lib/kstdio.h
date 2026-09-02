#ifndef KSTDIO_H
#define KSTDIO_H

#include <stdarg.h>
#include <stddef.h>

typedef enum {
    FORMAT_WIDTH,
    FORMAT_LENGTH,
    FORMAT_SPECIFIER,
    FORMAT_NORMAL,
    FORMAT_SHORT,
    FORMAT_SHORT_SHORT,
    FORMAT_LONG,
    FORMAT_LONG_LONG
} format_state_t;

typedef enum {
    FORMAT_LENGTH_DEFAULT,
    FORMAT_LENGTH_SHORT,
    FORMAT_LENGTH_SHORT_SHORT,
    FORMAT_LENGTH_LONG,
    FORMAT_LENGTH_LONG_LONG
} format_length_state_t;

void printf( const char* fmt, ... );
size_t kvformat(char* buf, size_t size, const char* fmt, va_list args);

#endif