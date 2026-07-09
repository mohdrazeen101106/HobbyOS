#include "../drivers/screen.h"
#include "./idt/idt.h"
#include "./pic/pic.h"
#include "./input/input.h"
#include "../drivers/keyboard.h"

// Testing shortcuts
void sample_callback() {
	print("Shortcut triggered\n");
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
	__asm__ volatile("sti");
}

void main() {
    clear_screen();
    print("Kernel loaded\n");

    input_init();
    init_interrupts();

    key_code_t combo[] = { KEY_LCTRL, KEY_LALT, KEY_LSHIFT };
    input_register_shortcut(combo, 3, sample_callback);

    input_event_t event;
    while (1) {
        input_update();                          
        while (input_pop_event(&event))
            if (event.type == INPUT_EVENT_KEY_PRESS && event.ascii)
                print_char(event.ascii, -1, -1, WHITE_ON_BLACK);       
        __asm__ volatile("hlt");
    }
}