#include "log.h"
#include "drivers/serial/serial.h"
#include "kernel/lib/kstdio.h"

static const char* level_names[] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

void klog(log_level_t level, const char* fmt, ...) {
    printf("[%s] %s\n", level_names[2], fmt);

    serial_write("[");
    serial_write(level_names[level]);
    serial_write("] ");

    serial_write(fmt);

    serial_write("\n");
}

void log_init(void) {
    // Further required changes will be added later
}