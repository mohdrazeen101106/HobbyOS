#include "kernel.h"
#include "./cpu/cpu.h"
#include "./idt/idt.h"
#include "./pic/pic.h"
#include "./input/input.h"
#include "./lib/kstdio.h"
#include "../drivers/screen/screen.h"
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/serial/serial.h"
#include "./log/log.h"
#include "./debug/debug.h"

// Testing shortcuts
void sample_callback() {
    debug_trigger_invalid_opcode();
}

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

    key_code_t combo[] = { KEY_LCTRL, KEY_LALT, KEY_LSHIFT };
    input_register_shortcut(combo, 3, sample_callback);

    input_event_t event;
    while (1) {
        input_update();                          
        while (input_pop_event(&event))
            if (event.type == INPUT_EVENT_KEY_PRESS && event.ascii)
                printf("%c", (char)event.ascii);
        cpu_hlt();
    }
}