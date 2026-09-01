#include "kernel/exceptions/exception_dispatch.h"
#include "kernel/exceptions/exception.h"
#include "kernel/panic/panic.h"

void exception_dispatch(interrupt_frame_t *frame)
{
    const exception_descriptor_t *desc = exception_get_descriptor(frame->vector);
    panic(desc, frame);
}