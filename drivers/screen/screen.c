#include "drivers/screen/screen.h"
#include "arch/x86/io/io.h"

/* Print a character on the screen at col, row, or at cursor position */
void print_char(uint8_t character, int col, int row, uint8_t attribute_byte) {
	/* Create a char(byte) pointer to the start of video memory */
	uint8_t *videomem = (uint8_t *) VIDEO_ADDRESS;

	/* If attribute byte is zero, assume the default style */
	if (!attribute_byte) attribute_byte = WHITE_ON_BLACK;

	/* Get the memory offset for the current location */
	uint16_t offset;

	if (col >= 0 && row >= 0)
		offset = get_screen_offset((uint8_t)col, (uint8_t)row);
	else offset = get_cursor();

	// Control characters
	if(character == '\n') {
		uint8_t rows = (uint8_t)(offset / (2*MAX_COLS));
		offset = get_screen_offset(79, rows);
	}
	else if(character == '\b') {
		uint8_t rows = (uint8_t)(offset / (2*MAX_COLS));
		uint8_t cols = (uint8_t)(offset % (2*MAX_COLS));

		
		offset -= 2;
		if(cols || rows) {
			videomem[offset] = ' ';
			videomem[offset+1] = attribute_byte;
			offset -= 2;
		}
	}
	else if(character == '\t') {
		uint8_t tab_space_count = 4;
		while(tab_space_count-- && offset < 2*MAX_COLS*MAX_ROWS) {
			videomem[offset] = ' ';
			videomem[offset+1] = attribute_byte;
			offset += 2;
		}
	}
	// Normal characters
	else {
		videomem[offset] = character;
		videomem[offset+1] = attribute_byte;
	}

	offset += 2; //Update the offset to the next character cell, i.e. two bytes ahead
	offset = handle_scrolling(offset); //If text overflows, then scroll
	set_cursor(offset); //Set the cursor to the new offset
}

uint16_t get_screen_offset(uint8_t column, uint8_t row) {
	return (uint16_t)(( row * MAX_COLS + column ) << 1);
}

uint16_t get_cursor() {
	port_byte_out(REG_SCREEN_CTRL, 14); // Select the register containing the upper byte
	uint16_t offset = port_byte_in(REG_SCREEN_DATA) << 8; // Get the data
	port_byte_out(REG_SCREEN_CTRL, 15); // Select the register containing the lower byte
	offset += port_byte_in(REG_SCREEN_DATA); // Get the data
	return offset << 1;
}

void set_cursor(uint16_t offset) {
	offset = offset >> 1; //Convert from character cell offset to number of characters written

	port_byte_out(REG_SCREEN_CTRL, 14); // Again, select the register containing the upper byte
	port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset >> 8)); // Set the data
	port_byte_out(REG_SCREEN_CTRL, 15); // Select the register containing the lower byte
	port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset & 255)); // Set the data
}

void clear_screen() {
	// Clear the screen by filling it with empty spaces
	for( uint8_t row = 0; row < MAX_ROWS ; ++row ) {
		for( uint8_t col = 0 ; col < MAX_COLS ; ++col ) {
			print_char(' ', col, row, WHITE_ON_BLACK);
		}
	}

	set_cursor(get_screen_offset(0,0));
}

void print_at(const uint8_t* message, int col, int row) {
	if (col >= 0 && row >= 0) set_cursor(get_screen_offset((uint8_t)col, (uint8_t)row));
	uint8_t i = 0;
	while(message[i]) print_char(message[i++], -1, -1, WHITE_ON_BLACK);
}

void print(const uint8_t* message) {
	print_at(message, -1, -1);
}

uint16_t handle_scrolling(uint16_t offset) {
	if(offset < 2 * MAX_COLS * MAX_ROWS) return offset;
	else {
		uint16_t offset_overflow = offset - (2 * MAX_COLS * MAX_ROWS);
		uint8_t rows_to_scroll = (uint8_t)(1 + (offset_overflow / (2 * MAX_COLS)));

		// Delete first (rows_to_scroll) rows and shift all other rows up, leaving new rows
		for( uint16_t i = 2 * rows_to_scroll * MAX_COLS; i < 2 * MAX_ROWS * MAX_COLS; i++ ) {
			uint8_t* video_mem = (uint8_t*) VIDEO_ADDRESS;
			video_mem[i - 2 * rows_to_scroll * MAX_COLS] = video_mem[i];
			video_mem[i] = 0;
		}
		return (uint16_t)(offset - 2 * MAX_COLS * rows_to_scroll);
	}
}