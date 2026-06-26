#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define WHITE_ON_BLACK 0x0f

#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

void print_char( char character, int col, int row, char attribute_byte );
int get_cursor();
void set_cursor( int offset );
unsigned int get_screen_offset(unsigned short column, unsigned short row );
void clear_screen();
void print_at( char* message, int col, int row );
void print( char* message );

