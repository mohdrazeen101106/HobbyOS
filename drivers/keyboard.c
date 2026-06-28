#include "keyboard.h"
#include "../kernel/io/io.h"

uint8_t read_scan_code() {
    return port_byte_in(KBD_DATA_PORT);
}