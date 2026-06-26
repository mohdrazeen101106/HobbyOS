/*
	kernel/low_level.h
	Contains low level functions that directly
	manipulate IO port registers
*/

unsigned char port_byte_in (unsigned short port);
void port_byte_out (unsigned short port, unsigned char data);
unsigned short port_word_in (unsigned short port);
void port_word_out (unsigned short port, unsigned short data);
