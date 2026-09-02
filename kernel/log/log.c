#include "kernel/log/log.h"
#include "drivers/serial/serial.h"
#include "drivers/screen/screen.h"
#include "kernel/lib/kstdio.h"
#include "kernel/lib/kstring.h"

#include <stdarg.h>
#include <stddef.h>

static const char* const level_names[] = {
    "[TRACE] ", 
    "[DEBUG] ", 
    "[INFO] ", 
    "[WARN] ", 
    "[ERROR] ", 
    "[FATAL] "
};

void klog(log_level_t level, const char* fmt, ...) {
    if ((size_t)level >= sizeof(level_names) / sizeof(level_names[0])) return;

    va_list args;
    va_start(args, fmt);

    char buffer[256] = {0};

    size_t pos = kstrlcpy(buffer, level_names[level], sizeof(buffer));
    if (pos > sizeof(buffer) - 1) pos = sizeof(buffer) - 1;  

    size_t written = kvformat(buffer + pos, sizeof(buffer) - pos, fmt, args);

    if (pos + written < sizeof(buffer) - 2) {
        buffer[pos + written]     = '\n';
        buffer[pos + written + 1] = '\0';
    }

    print((uint8_t*)buffer);
    serial_write(buffer);

    va_end(args);
}

void log_init(void) {
    // Further required changes will be added later
}