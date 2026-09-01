#ifndef IRQ_H
#define IRQ_H

#include "arch/x86/idt/idt.h"

void irq_dispatch(interrupt_frame_t *frame);

#endif