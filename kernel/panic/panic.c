#include "panic.h"

#include "../cpu/cpu.h"
#include "../log/log.h"

static volatile bool g_panicking = false;

bool panic_in_progress(void) {
    return g_panicking;
}

__attribute__((noreturn))
void panic(const exception_descriptor_t *desc, interrupt_frame_t *frame) {
    (void)frame;

    // Disable interrupts
    cpu_cli();

    // If the panic() function itself crashes, halt forever
    if (g_panicking)
        while (true) cpu_hlt();

    g_panicking = true;

    klog(LOG_FATAL, "KERNEL PANIC");

    if (desc != 0) {
        klog(LOG_FATAL, desc->name);
        klog(LOG_FATAL, desc->description);
    }
    else
        klog(LOG_FATAL, "Unknown CPU Exception");

    klog(LOG_FATAL, "System Halted.");
    
    while (true)
        cpu_hlt();    
}