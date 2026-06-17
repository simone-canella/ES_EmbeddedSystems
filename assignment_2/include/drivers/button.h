/**
 * @file button.h
 * @brief Header file for button initialization and debouncing logic.
 */

#ifndef BUTTON_H
#define BUTTON_H

/** * @brief The debounce delay threshold. 
 * Adjust based on your polling timer frequency.
 */
#define MAXDEBCYCLE 1 

/**
 * @struct ButtonState
 * @brief Tracks the persistent state and debounce count of a specific hardware button.
 */
typedef struct {
    int prev_state;       /**< The last verified stable state of the button. */
    int debounce_counter; /**< Tick down counter to validate stable pin reads. */
} ButtonState;

/**
 * @brief Initializes a button to its default (unpressed) state.
 * @param btn Pointer to the ButtonState structure to be initialized.
 */
void button_init(ButtonState *btn);

/**
 * @brief Evaluates the button state and handles debouncing.
 * @param btn Pointer to the active ButtonState structure tracking this pin.
 * @param current_pin_state The current digital reading of the physical button pin (0 or 1).
 * @return int Returns 1 if a valid press is detected, 0 otherwise.
 */
int button_has_just_been_pressed(ButtonState *btn, int current_pin_state);

#endif // BUTTON_H