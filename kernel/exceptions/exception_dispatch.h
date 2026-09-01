#ifndef EXCEPTION_DISPATCH_H
#define EXCEPTION_DISPATCH_H

#include "arch/x86/idt/idt.h"

void exception_dispatch(interrupt_frame_t *frame);

#endif