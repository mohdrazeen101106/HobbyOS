#ifndef KSTDIO_H
#define KSTDIO_H

typedef enum {
    PRINTF_LENGTH,
    PRINTF_SPECIFIER,
    PRINTF_NORMAL,
    PRINTF_SHORT,
    PRINTF_SHORT_SHORT,
    PRINTF_LONG,
    PRINTF_LONG_LONG
} printf_state_t;

typedef enum {
    PRINTF_LENGTH_DEFAULT,
    PRINTF_LENGTH_SHORT,
    PRINTF_LENGTH_SHORT_SHORT,
    PRINTF_LENGTH_LONG,
    PRINTF_LENGTH_LONG_LONG
} printf_length_state_t;

void printf( const char* fmt, ... );

#endif