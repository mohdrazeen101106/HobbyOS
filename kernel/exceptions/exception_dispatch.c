#include "kernel/exceptions/exception_dispatch.h"
#include "kernel/exceptions/exception.h"
#include "kernel/exceptions/stats.h"
#include "kernel/panic/panic.h"

void exception_dispatch(interrupt_frame_t *frame)
{
    exception_stats_record(frame->vector);

    const exception_descriptor_t *desc = exception_get_descriptor(frame->vector);
    panic(desc, frame);
}