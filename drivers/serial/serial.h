#ifndef SERIAL_H
#define SERIAL_H

#include <stdbool.h>

#define COM1_PORT 0x3F8

bool serial_init(void);

void serial_write_char(char character);
void serial_write(const char* message);

#endif