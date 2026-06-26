#include "../drivers/screen.h"
#include "./idt/idt.h"

void wrong_func() {
	char* video_memory = (char*) 0xb8000;
	*video_memory = 'Y';
	*(video_memory + 1) = 0x0f;
}

void main() {
	clear_screen();

	char *greeting_msg = "Hello, the kernel has loaded successfully!\n";
	print(greeting_msg);

	// Load Interrupts
	init_idt();
	print("\nInterrupts loaded.. \n");
}