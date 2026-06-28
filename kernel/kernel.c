#include "../drivers/screen.h"
#include "./idt/idt.h"
#include "./pic/pic.h"

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

	char *greeting_msg = "Hello, the kernel has loaded successfully!\n";
	print(greeting_msg);

	init_interrupts();
	print("\nInterrupts loaded.. \n");
}