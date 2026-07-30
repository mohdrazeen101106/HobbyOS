#ifndef DISPATCH_H
#define DISPATCH_H

#include "../idt/idt.h"

void interrupt_dispatch(interrupt_frame_t *frame);

#endif