#include "kernel/interrupts/dispatch.h"
#include "kernel/interrupts/irq.h"
#include "kernel/exceptions/exception_dispatch.h"

void interrupt_dispatch(interrupt_frame_t *frame)
{
    if(frame->vector < 32)
        exception_dispatch(frame);
    else if(frame->vector < 48)
        irq_dispatch(frame);
}