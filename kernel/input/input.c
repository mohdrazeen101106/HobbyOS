#include "input.h"

static input_event_t ring_buffer[INPUT_BUFFER_SIZE];

static volatile uint32_t head = 0; // Written by ISR, read by consumer
static volatile uint32_t tail = 0; // Written by consumer, read by ISR

/*
    Key pressed/release state derived entirely from the event stream.
    Updated inside input_push_event() before the event enters the ring buffer,
    so it always reflects the most current hardware state
*/
static bool pressed_keys[KEY_COUNT];

// Shortcut table
static shortcut_slot_t shortcuts[MAX_SHORTCUTS];

// Diagnostics
static input_stats_t stats;

/*
    void input_init()
    Initialize the input subsystem to a clean state.
    Resets the ring buffer, key state, shortcut table and diagnostic
    counters. Must be called before any events are pushed or popped.
*/
void input_init() {
    head = 0;
    tail = 0;
    stats.events_pushed = 0;
    stats.events_popped = 0;
    stats.events_dropped = 0;

    for (int i = 0; i < KEY_COUNT; i++) pressed_keys[i] = false;
    for (int i = 0; i < MAX_SHORTCUTS; i++) shortcuts[i].active = false;
}

/*
    bool input_push_event()
    Push an input event into the ring buffer. Called from the keyboard ISR. Updates
    the derived key state and resets shortcut triggered flags on key release.
    If the buffer is full the event is dropped and the drop counter is incremented. Callers
    should monitor input_get_stats().event_dropped.
*/
bool input_push_event(const input_event_t *event) {
    if(!event) return false;

    if(event->key != KEY_UNKNOWN && event->key < KEY_COUNT) {
        if (event->type == INPUT_EVENT_KEY_PRESS) pressed_keys[event->key] = true;
        else if (event->type == INPUT_EVENT_KEY_RELEASE) {
            pressed_keys[event->key] = false;
        
            for (int i = 0; i < MAX_SHORTCUTS; i++) {
                if (!shortcuts[i].active) continue;
                for (int j = 0; j < shortcuts[i].key_count; j++) {
                    if(shortcuts[i].keys[j] == event->key) {
                        shortcuts[i].triggered = false;
                        break;
                    }
                }
            }
        }
    }

    // Enqueue the event
    uint32_t cur_head = head;
    uint32_t cur_tail = tail;

    if((cur_head - cur_tail) >= INPUT_BUFFER_SIZE) {
        ++stats.events_dropped;
        return false;
    }

    ring_buffer[cur_head & INPUT_BUFFER_MASK] = *event;
    head = cur_head + 1;

    ++stats.events_pushed;
    return true;
}

/*
    bool input_pop_event()
    Pop the oldest event from the ring buffer. Returns false immediately if no
    events are pending. Called from the main loop
*/
bool input_pop_event(input_event_t *out) {
    if(!out) return false;

    uint32_t cur_head = head;
    uint32_t cur_tail = tail;

    if (cur_tail == cur_head) return false; // Empty event buffer

    *out = ring_buffer[cur_tail & INPUT_BUFFER_MASK];
    tail = cur_tail + 1;

    ++stats.events_popped;
    return true;
}

/*
    bool input_has_events()
    Check if any events are pending without consuming them
*/
bool input_has_events() {
    return head != tail;
}

/*
    uint32_t input_pending_count()
    Return the number of events currently in the buffer
*/
uint32_t input_pending_count() {
    return head - tail;
}

/*
    bool input_key_is_pressed()
    Query whether a key is pressed. This is from the derived state as per events fired
    from the keyboard ISR
*/
bool input_key_is_pressed(key_code_t key) {
    if (key == KEY_UNKNOWN || key >= KEY_COUNT) return false;
    return pressed_keys[key];
}

/*
    uint32_t input_register_shortcut()
    Register a key combination shortcut and its callback. The callback fires exactly once
    each time all the keys are held simultaneously. It will not fire again until at least one key
    is released and the order is not important. 
*/
int input_register_shortcut(const key_code_t *keys, uint8_t key_count, void (*callback)(void)) {
    if (!keys || !callback) return INPUT_ERR_INVALID_ARG;
    if (key_count == 0 || key_count > MAX_SHORTCUT_KEYS) return INPUT_ERR_INVALID_ARG;

    for (int i = 0; i < MAX_SHORTCUTS; i++) {
        if (shortcuts[i].active) continue;

        for (int j = 0; j < key_count; j++) shortcuts[i].keys[j] = keys[j];
        
        shortcuts[i].key_count = key_count;
        shortcuts[i].active = true;
        shortcuts[i].callback = callback;
        shortcuts[i].triggered = false;

        return i;
    }

    return INPUT_ERR_SHORTCUT_FULL;
}

/*
    void input_deregister_shortcut()
    Deregister a previously registered shortcut. The slot is freed immediately.
*/
input_error_t input_deregister_shortcut(uint32_t id) {
    if(id >= MAX_SHORTCUTS) return INPUT_ERR_INVALID_ARG;
    shortcuts[id].active = false;
    shortcuts[id].triggered = false;
    return INPUT_OK;
}

/*
    Check all registered shortcuts and fire active combinations.
    Must be called from the main loop, NOT from an ISR. Iterates all
    active shortcuts and fires the callback of any whose keys are all
    currently held in pressed_keys[]. The triggered flag ensures each
    combination fires at most once per press event.
*/
void input_update() {
    for (int i = 0; i < MAX_SHORTCUTS; i++) {
        if (!shortcuts[i].active || shortcuts[i].triggered)
            continue;
 
        bool all_pressed = true;
        for (int k = 0; k < shortcuts[i].key_count; k++) {
            if (!pressed_keys[shortcuts[i].keys[k]]) {
                all_pressed = false;
                break;
            }
        }
 
        if (all_pressed) {
            shortcuts[i].triggered = true;
            shortcuts[i].callback();
        }
    }
}

/* 
    void input_get_stats()
    Get the current event counter values into the diagnostic structure
*/
void input_get_stats(input_stats_t *out) {
    if (!out) return;
    *out = stats;
}