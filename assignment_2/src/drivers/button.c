/**
 * @file button.c
 * @brief Implementation of button initialization and debouncing logic.
 */

#include "button.h"

/**
 * @brief Initializes a button to its default (unpressed) state.
 * * Sets the previous state to 1, assuming pull-up resistors are used, and resets the 
 * debounce counter to MAXDEBCYCLE.
 * * @param btn Pointer to the ButtonState structure to be initialized.
 */
// Initialize a button to its default (unpressed) state
void button_init(ButtonState *btn) {
    btn->prev_state = 1; // Since pull-up resistors, 1 = unpressed
    btn->debounce_counter = MAXDEBCYCLE;
}

/**
 * @brief Evaluates the button state and handles debouncing.
 * * Returns 1 ONLY on the exact cycle the button is validly pressed.
 * Returns 0 otherwise.
 * Decrements the debounce counter if a state change is detected, and updates 
 * the state only when the counter reaches 0.
 * * @param btn Pointer to the active ButtonState structure tracking this pin.
 * @param current_pin_state The current digital reading of the physical button pin (0 or 1).
 * @return int Returns 1 if a valid press is detected, 0 otherwise.
 */
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