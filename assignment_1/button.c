#include "button.h"

// Initialize a button to its default (unpressed) state
void button_init(ButtonState *btn) {
    btn->prev_state = 1; // Since pull-up resistors, 1 = unpressed
    btn->debounce_counter = MAXDEBCYCLE;
}

// Returns 1 ONLY on the exact cycle the button is validly pressed.
// Returns 0 otherwise.
int button_has_just_been_pressed(ButtonState *btn, int current_pin_state) {
    int triggered = 0; // Default to no action

    // Check for a press
    if ((current_pin_state == 0) && (btn->prev_state == 1)) {
        if (btn->debounce_counter == 0) {
            triggered = 1;  // Valid press detected!
            btn->debounce_counter = MAXDEBCYCLE;
            btn->prev_state = 0;
        } else {
            btn->debounce_counter--;
        }
    } 
    
    // Check for a release
    else if ((current_pin_state == 1) && (btn->prev_state == 0)) {
        if (btn->debounce_counter == 0) {
            btn->debounce_counter = MAXDEBCYCLE;
            btn->prev_state = 1; // Valid release detected
        } else {
            btn->debounce_counter--;
        }
    }

    return triggered;
}