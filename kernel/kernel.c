#include "kernel/kernel.h"
#include "arch/x86/cpu/cpu.h"
#include "arch/x86/idt/idt.h"
#include "arch/x86/pic/pic.h"
#include "kernel/input/input.h"
#include "kernel/lib/kstdio.h"
#include "drivers/screen/screen.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/serial/serial.h"
#include "kernel/log/log.h"
#include "tests/debug/debug.h"

/*
	void init_interrupts()
	A function that remaps the PIC vector offsets for hardware IRQs, loads
	the Interrupt Descriptor Table IDT into memory and re-enables interrupts.
	Required for essential functions like timers, keyboard input, etc.
*/
void init_interrupts() {
	pic_remap(); // Hardware IRQs
	init_idt();
}

void kmain() {
    serial_init();
    clear_screen();
    log_init();

    klog(LOG_INFO, "Entered Kernel");

    klog(LOG_INFO, "Screen Initialized");

    printf("\nHello, Welcome to HobbyOS!\n\n");

    input_init();
    init_interrupts();
    klog(LOG_INFO, "Interrupts Enabled");

    // Register shortcuts for kernel panic testing
    key_code_t c1[] = { KEY_LCTRL, KEY_LALT, KEY_LSHIFT };
    key_code_t c2[] = { KEY_LCTRL, KEY_LALT, KEY_TAB };
    input_register_shortcut(c1, 3, debug_trigger_invalid_opcode);
    input_register_shortcut(c2, 4, debug_trigger_gen_protection_fault);

    // klog(LOG_DEBUG, "This should be 2: %d", 2);

    input_event_t event;
    while (1) {
        input_update();                          
        while (input_pop_event(&event))
            if (event.type == INPUT_EVENT_KEY_PRESS && event.ascii)
                printf("%c", (char)event.ascii);
        cpu_hlt();
    }
}