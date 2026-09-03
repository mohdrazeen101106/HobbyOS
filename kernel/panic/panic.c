#include "kernel/panic/panic.h"
#include "kernel/panic/diagnostics.h"
#include "arch/x86/cpu/cpu.h"
#include "kernel/log/log.h"

static volatile bool g_panicking = false;

bool panic_in_progress(void) {
    return g_panicking;
}

__attribute__((noreturn))
void panic(const exception_descriptor_t *desc, interrupt_frame_t *frame) {
    // Disable interrupts
    cpu_cli();

    // If the panic() function itself crashes, halt forever
    if (g_panicking)
        while (true) cpu_hlt();

    g_panicking = true;

    klog(LOG_FATAL, "KERNEL PANIC");

    // Generate a proper exception report
    panic_report(desc, frame);
    
    while (true)
        cpu_hlt();    
}