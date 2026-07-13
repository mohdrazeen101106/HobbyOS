#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdbool.h>

#include "../../drivers/keyboard.h"

// Ring Buffer Config

/*
    Ring buffer capacity. Must be a power of 2 for bitmask indexing.
    64 slots comfortably absorbs a fast typist even if the main loop is briefly
    busy.
*/
#define INPUT_BUFFER_SIZE   64U
#define INPUT_BUFFER_MASK   (INPUT_BUFFER_SIZE - 1U)

// Shortcut Config

#define MAX_SHORTCUTS       32
#define MAX_SHORTCUT_KEYS   4

// Event Types

typedef enum {
    INPUT_EVENT_KEY_PRESS   = 0,
    INPUT_EVENT_KEY_RELEASE = 1,
} input_event_type_t;

// Input event structure

/*
    A single keyboard input event with full context. Modifier state
    is captured at the moment of the event so consumers processing
    events asynchronously always see the correct context.
*/
typedef struct {
    input_event_type_t  type;       // Keyboard press or release
    key_code_t          key;        // The keycode
    uint8_t             ascii;      // The ascii value of the keycode
    kbd_state_t         modifiers;  // Keyboard modifiers
} input_event_t;

// Shortcut table slot
typedef struct {
    key_code_t keys[MAX_SHORTCUT_KEYS]; // Required keys for the shortcut
    uint8_t key_count;                  // Number of required keys
    void (*callback)(void);             // Fired once all the keys are pressed
    bool triggered;                     // Blocks repetitive calls when keys are held
    bool active;                        // Whether this slot is in use or not
} shortcut_slot_t;

// Diagnostic Stats

typedef struct {
    uint32_t events_pushed;     // Events successfully produced
    uint32_t events_popped;     // Events successfully consumed
    uint32_t events_dropped;    // Events dropped because the buffer was full
} input_stats_t;

// Input Error Codes

typedef enum {
    INPUT_OK                = 0,
    INPUT_ERR_SHORTCUT_FULL = -1,   // Shortcut table is full
    INPUT_ERR_INVALID_ARG   = -2,   // Invalid argument or pointer
} input_error_t;

void input_init(void);
bool input_push_event(const input_event_t *event);
bool input_pop_event(input_event_t *out);
bool input_has_events(void);
uint32_t input_pending_count(void);
bool input_key_is_pressed(key_code_t key);

int input_register_shortcut(const key_code_t *keys, uint8_t key_count, void (*callback)(void));
input_error_t input_deregister_shortcut(uint32_t id);

void input_update(void);

void input_get_stats(input_stats_t *out);

#endif