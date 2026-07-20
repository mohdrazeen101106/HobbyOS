#include "io.h"
#include <stdint.h>

uint8_t port_byte_in (uint16_t port) {
	// A handy C wrapper function that reads a byte from the specified port
	// "=a" (result) means: put AL register in variable RESULT when finished
	// "d" (port) means: load EDX with port
	uint8_t result;
	__asm__ volatile("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

void port_byte_out (uint16_t port, uint8_t data) {
	// "a" (data) means: load EAX with data
	// "d' (port) means: load EDX with port
	__asm__ volatile("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t port_word_in (uint16_t port) {
	uint16_t result;
	__asm__ volatile("in %%dx, %%ax" : "=a" (result) : "d" (port));
	return result;
}

void port_word_out (uint16_t port, uint16_t data) {
	__asm__ volatile("out %%ax, %%dx" : : "a" (data), "d" (port));
}
