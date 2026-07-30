#ifndef PANIC_H
#define PANIC_H

#include <stdbool.h>

#include "../exceptions/exception.h"
#include "../idt/idt.h"

// Enter the kernel panic state, this function never returns
__attribute__((noreturn))
void panic(const exception_descriptor_t *desc, interrupt_frame_t *frame);

// Check if the kernel is currently panicking
bool panic_in_progress(void);

#endif