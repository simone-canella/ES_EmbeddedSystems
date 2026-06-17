#ifndef PWM_H
#define PWM_H

#include <math.h>
#include <stdbool.h>
#include <xc.h>

// Function Prototypes
void pwm_setup(void);
int pwm_set_duty_cycle(int percent, unsigned int reg);
int clamp(int value);
void execute_motion(int v, int w);

#endif