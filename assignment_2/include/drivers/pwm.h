/**
 * @file pwm.h
 * @brief Header file for Pulse Width Modulation (PWM) and motor control interface.
 */

#ifndef PWM_H
#define PWM_H

#include "config.h"

/**
 * @brief Initializes the motor control pins and Output Compare peripherals.
 */
void pwm_init(void);

/**
 * @brief Converts a percentage into a hardware register duty cycle value.
 * @param percent The desired duty cycle ranging from 0 to 100.
 * @param reg The period register boundary (OCxRS) to scale against.
 * @return int The calculated hardware integer to be written to OCxR.
 */
int pwm_set_duty_cycle(int percent, unsigned int reg);

/**
 * @brief Computes differential kinematics and updates H-Bridge duty cycles.
 * @param v Target linear velocity (-100 to 100).
 * @param w Target angular yaw rate (-100 to 100).
 */
void execute_motion(int v, int w);

#endif // PWM_H