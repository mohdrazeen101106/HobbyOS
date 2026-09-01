#include "kernel/interrupts/irq.h"
#include "arch/x86/pic/pic.h"
#include "drivers/keyboard/keyboard.h"

void irq_dispatch(interrupt_frame_t *frame)
{
    switch(frame->vector)
    {
        case 33:
            keyboard_handler();
            break;
    }

    pic_send_eoi(frame->vector - 32);
}