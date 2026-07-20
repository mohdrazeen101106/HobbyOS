#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define WHITE_ON_BLACK 0x0f

#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

void print_char( uint8_t character, int col, int row, uint8_t attribute_byte );
uint16_t get_cursor( void );
void set_cursor( uint16_t offset );
uint16_t get_screen_offset( uint8_t column, uint8_t row );
void clear_screen( void );
void print_at( const uint8_t* message, int col, int row );
void print( const uint8_t* message );

#endif