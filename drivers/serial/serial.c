#include "serial.h"
#include "../../kernel/io/io.h"
#include <stdint.h>

static bool serial_initialized = false;

bool serial_init(void) {
    port_byte_out(COM1_PORT + 1, 0x00); // Disable all interrupts
    port_byte_out(COM1_PORT + 3, 0x80); // Enable DLAB
    
    // Set baud rate to 115200 / 3 = 38400
    port_byte_out(COM1_PORT, 0x03); // Low byte
    port_byte_out(COM1_PORT + 1, 0x00); // High byte
    
    // Line Control Register
    port_byte_out(COM1_PORT + 3, 0x03); // 8N1 standard

    // FIFO Control Register
    port_byte_out(COM1_PORT + 2, 0xC7); // Enable FIFO, clear transmission and receiver queues, 14-byte buffer size
    
    // Modem Control Register
    port_byte_out(COM1_PORT + 4, 0x0B); // Set the DTR and RTS pins, also enable IRQ

    serial_initialized = true;

    return true;
}

void serial_write_char(char character) {
    if(!serial_initialized) return;

    // Wait for any pending writes to finish
    while(!(port_byte_in(COM1_PORT + 5) & 0x20));

    port_byte_out(COM1_PORT, (uint8_t)character);
}

void serial_write(const char* message) {
    while(*message) {
        if(*message == '\n') serial_write_char('\r');
        serial_write_char(*message);
        ++message;
    }
}