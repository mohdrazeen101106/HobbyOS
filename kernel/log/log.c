#include "log.h"
#include "../../drivers/screen/screen.h"
#include "../../drivers/serial/serial.h"

static const char* level_names[] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

void klog(log_level_t level, const char* fmt, ...) {
    (void) level;

    print((uint8_t *)"[");
    print((uint8_t *)level_names[level]);
    print((uint8_t *)"] ");

    print((uint8_t *)fmt);

    print((uint8_t *)"\n");

    serial_write("[");
    serial_write(level_names[level]);
    serial_write("] ");

    serial_write(fmt);

    serial_write("\n");
}

void log_init(void) {
    // Further required changes will be added later
}