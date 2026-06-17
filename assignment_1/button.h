#ifndef BUTTON_H
#define BUTTON_H

// The debounce delay (adjust based on your timer frequency)
#define MAXDEBCYCLE 1 

typedef struct {
    int prev_state;
    int debounce_counter;
} ButtonState;

// Function prototypes
void button_init(ButtonState *btn);
int button_has_just_been_pressed(ButtonState *btn, int current_pin_state);

#endif // BUTTON_H