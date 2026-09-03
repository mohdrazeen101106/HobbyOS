#ifndef PANIC_DIAGNOSTICS_H
#define PANIC_DIAGNOSTICS_H

#include "arch/x86/idt/idt.h"
#include "kernel/exceptions/exception.h"

void panic_report(const exception_descriptor_t* desc, const interrupt_frame_t* frame);

#endif