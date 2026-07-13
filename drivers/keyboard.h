#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>

#define KBD_DATA_PORT 0x60

#define SC1_BREAK_BIT 0x80
#define SC1_EXTENDED_PREFIX 0xE0

typedef enum {
    KBD_OK                      = 0,
    KBD_ERR_PS2_INIT            = -1,
    KBD_ERR_PS2_TIMEOUT         = -2,
    KBD_ERR_INVALIID_ARG        = -3,
} kbd_err_t;

typedef enum {
    STATE_NORMAL,   // Waiting for a standard or 0xE0 byte
    STATE_EXTENDED  // Received a 0xE0 byte, now waiting for the key code
} parse_state_t;

typedef enum {
    KEY_UNKNOWN = 0,

    KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H,
    KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P,
    KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X,
    KEY_Y, KEY_Z,

    KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7,
    KEY_8, KEY_9,

    KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7,
    KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,

    KEY_LSHIFT, KEY_RSHIFT, KEY_LCTRL, KEY_RCTRL, KEY_LALT,
    KEY_RALT, KEY_CAPS, KEY_NMLOCK, KEY_SCLOCK,

    KEY_ESC, KEY_TAB, KEY_BACKSPACE, KEY_ENTER, KEY_SPACE,

    KEY_ARROW_UP, KEY_ARROW_DOWN, KEY_ARROW_LEFT, KEY_ARROW_RIGHT,
    KEY_HOME, KEY_END, KEY_PAGE_UP, KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE,

    KEY_BACKTICK, KEY_MINUS, KEY_EQUAL, KEY_LBRACKET, KEY_RBACKET, KEY_BACKSLASH,
    KEY_SEMICOLON, KEY_APOSTROPHE, KEY_COMMA, KEY_FULLSTOP, KEY_FORSLASH,

    KEY_K0, KEY_KFSTOP, KEY_K1, KEY_K2, KEY_K3, KEY_K4, KEY_K5, KEY_K6, KEY_K7, KEY_K8,
    KEY_K9, KEY_KMINUS, KEY_KPLUS, KEY_KASTERISK,

    KEY_COUNT // Always keep last
} key_code_t;

/*
    struct kbd_state_t
    Stores the current keyboard state, i.e. whether caps lock, etc. are pressed
*/
typedef struct
{
    bool shift;
    bool caps_lock;
    bool ctrl;
    bool alt;
} kbd_state_t;

kbd_err_t keyboard_init(void);
void keyboard_handler(void);

bool key_is_pressed(key_code_t key);
const kbd_state_t *kbd_get_state(void);

#endif