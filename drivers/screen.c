#include "screen.h"
#include "../kernel/low_level/low_level.h"

/* Print a character on the screen at col, row, or at cursor position */
void print_char(char character, int col, int row, char attribute_byte) {
	/* Create a char(byte) pointer to the start of video memory */
	unsigned char *videomem = (unsigned char *) VIDEO_ADDRESS;

	/* If attribute byte is zero, assume the default style */
	if (!attribute_byte) attribute_byte = WHITE_ON_BLACK;

	/* Get the memory offset for the current location */
	int offset;

	if (col >= 0 && row >= 0)
		offset = get_screen_offset(col, row);
	else offset = get_cursor();

	if (character == '\n') {
		int rows = offset / (2*MAX_COLS);
		offset = get_screen_offset(79, rows);
	}
	else {
		videomem[offset] = character;
		videomem[offset+1] = attribute_byte;
	}

	offset += 2; //Update the offset to the next character cell, i.e. two bytes ahead
	// offset = handle_scrolling(offset); //If text overflows, then scroll
	set_cursor(offset); //Set the cursor to the new offset
}

unsigned int get_screen_offset(unsigned short column, unsigned short row) {
	return ( row * MAX_COLS + column ) * 2;
}

int get_cursor() {
	port_byte_out(REG_SCREEN_CTRL, 14); // Select the register containing the upper byte
	int offset = port_byte_in(REG_SCREEN_DATA) << 8; // Get the data
	port_byte_out(REG_SCREEN_CTRL, 15); // Select the register containing the lower byte
	offset += port_byte_in(REG_SCREEN_DATA); // Get the data
	return offset*2;
}

void set_cursor(int offset) {
	offset /= 2; //Convert from character cell offset to number of characters written

	port_byte_out(REG_SCREEN_CTRL, 14); // Again, select the register containing the upper byte
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8)); // Set the data
	port_byte_out(REG_SCREEN_CTRL, 15); // Select the register containing the lower byte
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 255)); // Set the data
}

void clear_screen() {
	// Clear the screen by filling it with empty spaces
	for( int row = 0; row < MAX_ROWS ; ++row ) {
		for( int col = 0 ; col < MAX_COLS ; ++col ) {
			print_char(' ', col, row, WHITE_ON_BLACK);
		}
	}

	set_cursor(get_screen_offset(0,0));
}

void print_at(char* message, int col, int row) {
	if (col >= 0 && row >= 0) set_cursor(get_screen_offset(col, row));
	int i = 0;
	while(message[i]) print_char(message[i++], col, row, WHITE_ON_BLACK);
}

void print(char* message) {
	print_at(message, -1, -1);
}
