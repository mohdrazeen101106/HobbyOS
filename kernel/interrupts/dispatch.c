#include "dispatch.h"

#include "../pic/pic.h"
#include "../exceptions/exception.h"
#include "../panic/panic.h"
#include "../../drivers/keyboard/keyboard.h"

static void irq_dispatch(interrupt_frame_t *frame);
static void exception_dispatch(interrupt_frame_t *frame);

void interrupt_dispatch(interrupt_frame_t *frame)
{
    if(frame->vector < 32)
        exception_dispatch(frame);
    else if(frame->vector < 48)
        irq_dispatch(frame);
}

static void irq_dispatch(interrupt_frame_t *frame)
{
    switch(frame->vector)
    {
        case 33:
            keyboard_handler();
            break;
    }

    pic_send_eoi(frame->vector - 32);
}

static void exception_dispatch(interrupt_frame_t *frame)
{
    const exception_descriptor_t *desc = exception_get_descriptor(frame->vector);
    panic(desc, frame);
}