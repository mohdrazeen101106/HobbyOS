#include "keyboard.h"
#include "../kernel/io/io.h"
#include "../kernel/input/input.h"

/* Lookup Tables */

// Standard set 1 make codes to key_code_t key codes
static const key_code_t sc1_to_key[256] = {
    [0x01] = KEY_ESC,       [0x02] = KEY_1,
    [0x03] = KEY_2,         [0x04] = KEY_3,
    [0x05] = KEY_4,         [0x06] = KEY_5,
    [0x07] = KEY_6,         [0x08] = KEY_7,
    [0x09] = KEY_8,         [0x0A] = KEY_9,
    [0x0B] = KEY_0,         [0x0C] = KEY_MINUS,
    [0x0D] = KEY_EQUAL,     [0x0E] = KEY_BACKSPACE,
    [0x0F] = KEY_TAB,       [0x10] = KEY_Q,
    [0x11] = KEY_W,         [0x12] = KEY_E,
    [0x13] = KEY_R,         [0x14] = KEY_T,
    [0x15] = KEY_Y,         [0x16] = KEY_U,
    [0x17] = KEY_I,         [0x18] = KEY_O,
    [0x19] = KEY_P,         [0x1A] = KEY_LBRACKET,
    [0x1B] = KEY_RBACKET,   [0x1C] = KEY_ENTER,
    [0x1D] = KEY_LCTRL,     [0x1E] = KEY_A,
    [0x1F] = KEY_S,         [0x20] = KEY_D,
    [0x21] = KEY_F,         [0x22] = KEY_G,
    [0x23] = KEY_H,         [0x24] = KEY_J,
    [0x25] = KEY_K,         [0x26] = KEY_L,
    [0x27] = KEY_SEMICOLON, [0x28] = KEY_APOSTROPHE,
    [0x29] = KEY_BACKTICK,  [0x2A] = KEY_LSHIFT,
    [0x2B] = KEY_BACKSLASH, [0x2C] = KEY_Z,
    [0x2D] = KEY_X,         [0x2E] = KEY_C,
    [0x2F] = KEY_V,         [0x30] = KEY_B,
    [0x31] = KEY_N,         [0x32] = KEY_M,
    [0x33] = KEY_COMMA,     [0x34] = KEY_FULLSTOP,
    [0x35] = KEY_FORSLASH,  [0x36] = KEY_RSHIFT,
    [0x37] = KEY_KASTERISK, [0x38] = KEY_LALT,
    [0x39] = KEY_SPACE,     [0x3A] = KEY_CAPS,
    [0x3B] = KEY_F1,        [0x3C] = KEY_F2,
    [0x3D] = KEY_F3,        [0x3E] = KEY_F4,
    [0x3F] = KEY_F5,        [0x40] = KEY_F6,
    [0x41] = KEY_F7,        [0x42] = KEY_F8,
    [0x43] = KEY_F9,        [0x44] = KEY_F10,
    [0x45] = KEY_NMLOCK,    [0x46] = KEY_SCLOCK,
    [0x47] = KEY_K7,        [0x48] = KEY_K8,
    [0x49] = KEY_K9,        [0x4A] = KEY_KMINUS,
    [0x4B] = KEY_K4,        [0x4C] = KEY_K5,
    [0x4D] = KEY_K6,        [0x4E] = KEY_KPLUS,
    [0x4F] = KEY_K1,        [0x50] = KEY_K2,
    [0x51] = KEY_K3,        [0x52] = KEY_K0,
    [0x53] = KEY_KFSTOP,    [0x57] = KEY_F11,
    [0x58] = KEY_F12,
};

// Extended set 1 make codes to key_code_t key codes
static const key_code_t sc1_ext_to_key[256] = {
    [0x1D] = KEY_RCTRL,         [0x38] = KEY_RALT,
    [0x47] = KEY_HOME,          [0x48] = KEY_ARROW_UP,
    [0x49] = KEY_PAGE_UP,       [0x4B] = KEY_ARROW_LEFT,
    [0x4D] = KEY_ARROW_RIGHT,   [0x4F] = KEY_END,
    [0x50] = KEY_ARROW_DOWN,    [0x51] = KEY_PAGE_DOWN,
    [0x52] = KEY_INSERT,        [0x53] = KEY_DELETE
};
 
// Set 1 make codes → ASCII 
static const uint8_t sc1_lower[256] = {
    [0x02]='1', [0x03]='2', [0x04]='3', [0x05]='4', [0x06]='5',
    [0x07]='6', [0x08]='7', [0x09]='8', [0x0A]='9', [0x0B]='0',
    [0x0C]='-', [0x0D]='=', [0x0E]='\b',[0x0F]='\t',
    [0x10]='q', [0x11]='w', [0x12]='e', [0x13]='r', [0x14]='t',
    [0x15]='y', [0x16]='u', [0x17]='i', [0x18]='o', [0x19]='p',
    [0x1A]='[', [0x1B]=']', [0x1C]='\n',
    [0x1E]='a', [0x1F]='s', [0x20]='d', [0x21]='f', [0x22]='g',
    [0x23]='h', [0x24]='j', [0x25]='k', [0x26]='l', [0x27]=';',
    [0x28]='\'', [0x29]='`', [0x2B]='\\',
    [0x2C]='z', [0x2D]='x', [0x2E]='c', [0x2F]='v', [0x30]='b',
    [0x31]='n', [0x32]='m', [0x33]=',', [0x34]='.', [0x35]='/',
    [0x39]=' ',
};
 
// Set 1 make codes → ASCII shifted
static const uint8_t sc1_upper[256] = {
    [0x02]='!', [0x03]='@', [0x04]='#', [0x05]='$', [0x06]='%',
    [0x07]='^', [0x08]='&', [0x09]='*', [0x0A]='(', [0x0B]=')',
    [0x0C]='_', [0x0D]='+', [0x0E]='\b',[0x0F]='\t',
    [0x10]='Q', [0x11]='W', [0x12]='E', [0x13]='R', [0x14]='T',
    [0x15]='Y', [0x16]='U', [0x17]='I', [0x18]='O', [0x19]='P',
    [0x1A]='{', [0x1B]='}', [0x1C]='\n',
    [0x1E]='A', [0x1F]='S', [0x20]='D', [0x21]='F', [0x22]='G',
    [0x23]='H', [0x24]='J', [0x25]='K', [0x26]='L', [0x27]=':',
    [0x28]='"', [0x29]='~', [0x2B]='|',
    [0x2C]='Z', [0x2D]='X', [0x2E]='C', [0x2F]='V', [0x30]='B',
    [0x31]='N', [0x32]='M', [0x33]='<', [0x34]='>', [0x35]='?',
    [0x39]=' ',
};

static parse_state_t parse_state = STATE_NORMAL;

// Store whether each key is pressed
// 0x00 for unpressed and 0xFF for pressed!
static volatile bool key_state[KEY_COUNT];

static kbd_state_t kbd_state; // Store keyboard modifier states, shift etc.

// ASCII Resolution helpers

static inline uint8_t is_letter(uint8_t c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static uint8_t resolve_ascii(uint8_t scancode) {
    uint8_t c = kbd_state.shift ? sc1_upper[scancode] : sc1_lower[scancode];
    if (kbd_state.caps_lock && c && is_letter(c))
        c = kbd_state.shift ? sc1_lower[scancode] : sc1_upper[scancode];
    return c; 
}

/* Key State Update */
 
/**
 * Record a key press, sync modifier state, push a KEY_PRESS event.
 */
static void key_set_pressed(key_code_t key, uint8_t scancode) {
    if (key == KEY_UNKNOWN) return;
 
    key_state[key] = true;
 
    switch (key) {
        case KEY_LSHIFT:
        case KEY_RSHIFT:    kbd_state.shift     = true;
                            break;
        case KEY_CAPS:      kbd_state.caps_lock = !kbd_state.caps_lock; 
                            break;
        case KEY_LCTRL:
        case KEY_RCTRL:     kbd_state.ctrl      = true;                 
                            break;
        case KEY_LALT:  
        case KEY_RALT:      kbd_state.alt       = true;                 
                            break;
        default:            break;
    }
 
    input_event_t event = {
        .type      = INPUT_EVENT_KEY_PRESS,
        .key       = key,
        .ascii     = resolve_ascii(scancode),
        .modifiers = kbd_state,
    };
    input_push_event(&event);
}

/*
    Record a key release, sync modifier state, push a KEY_RELEASE event.
    Modifier keys only clear when all physical keys for that modifier are up,
    e.g. releasing left shift while right shift is held keeps shift active.
*/
static void key_set_released(key_code_t key) {
    if (key == KEY_UNKNOWN) return;
 
    key_state[key] = false;
 
    switch (key) {
        case KEY_LSHIFT:
        case KEY_RSHIFT:
            if (!key_state[KEY_LSHIFT] && !key_state[KEY_RSHIFT])
                kbd_state.shift = false;
            break;
        case KEY_LCTRL:
        case KEY_RCTRL:
            if (!key_state[KEY_LCTRL] && !key_state[KEY_RCTRL])
                kbd_state.ctrl = false;
            break;
        case KEY_LALT:
        case KEY_RALT:
            if (!key_state[KEY_LALT] && !key_state[KEY_RALT])
                kbd_state.alt = false;
            break;
        default:
            break;
    }
 
    input_event_t event = {
        .type      = INPUT_EVENT_KEY_RELEASE,
        .key       = key,
        .ascii     = 0,
        .modifiers = kbd_state,
    };
    input_push_event(&event);
}
  
/*
    IRQ1 keyboard interrupt handler (scancode set 1).

    Reads exactly one byte from the PS/2 data port and advances the
    two-state parser. A set 1 extended sequence (0xE0 + code) spans
    two IRQs; all other events complete in a single IRQ.

    Runs in interrupt context. Does not block, print, or call
    any function that may sleep or take a lock.
*/
void keyboard_handler() {
    uint8_t byte = port_byte_in(KBD_DATA_PORT);
 
    switch (parse_state) {
        case STATE_NORMAL:
            if (byte == SC1_EXTENDED_PREFIX)
                parse_state = STATE_EXTENDED;
            else if (byte & SC1_BREAK_BIT)
                // Break code has same index as make code with bit 7 set
                key_set_released(sc1_to_key[byte & ~SC1_BREAK_BIT]);
            else
                key_set_pressed(sc1_to_key[byte], byte);

            break;
 
        case STATE_EXTENDED:
            if (byte & SC1_BREAK_BIT)
                key_set_released(sc1_ext_to_key[byte & ~SC1_BREAK_BIT]);
            else
                key_set_pressed(sc1_ext_to_key[byte], 0);

            parse_state = STATE_NORMAL;
            break;
    }
 
}
 
/*
    Query whether a physical key is currently held down.
    Reads directly from the ISR-maintained key_state_t bitmap.
 */
bool key_is_pressed(key_code_t key) {
    if (key == KEY_UNKNOWN || key >= KEY_COUNT) return 0;
    return key_state[key];
}
 
/*
    Return a pointer to the current modifier state.
*/
const kbd_state_t *kbd_get_state() {
    return &kbd_state;
}

/*
    Initialize keyboard state
*/
kbd_err_t keyboard_init() {
    kbd_state.alt = false;
    kbd_state.shift = false;
    kbd_state.caps_lock = false;
    kbd_state.ctrl = false;

    return KBD_OK;
}