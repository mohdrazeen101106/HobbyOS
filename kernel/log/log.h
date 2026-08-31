#ifndef LOG_H
#define LOG_H

#include <stdarg.h>

typedef enum {
    LOG_TRACE = 0,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
} log_level_t;

void log_init(void);
void klog(log_level_t level, const char* fmt, ...);

#endif