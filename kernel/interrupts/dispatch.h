#ifndef DISPATCH_H
#define DISPATCH_H

#include "arch/x86/idt/idt.h"

void interrupt_dispatch(interrupt_frame_t *frame);

#endif