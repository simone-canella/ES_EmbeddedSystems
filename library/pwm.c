#include "pwm.h"

#define F_CY 72000000UL // 72MHz Internal Instruction Clock
#define F_PWM 10000UL	// 10KHz
#define PRESCALER 1		//

/**
 * @brief Initializes the motor control pins and Output Compare peripherals.
 *
 * @details Configures the directional pins (RD1-RD4) as outputs and maps them to 
 * specific Re-mappable Pins (RP65-RP68). It initializes four 
 * Output Compare modules (OC1-OC4) in edge-aligned PWM mode with a base 
 * frequency of 10 kHz.
 */
void pwm_setup(void) {

	// PIN SETUP
	TRISEbits.TRISE8 = 1; // configure pin RE8 as input

	// MOTOR PINS CONFIGURATION
	LATD = 0x0000;		  // MEMO: first setup and then "activate"
	TRISDbits.TRISD1 = 0; // RD1 -> left-side backward motion
	TRISDbits.TRISD2 = 0; // RD2 -> left-side forward motion
	TRISDbits.TRISD3 = 0; // RD3 -> right-side backward motion
	TRISDbits.TRISD4 = 0; // RD4 -> right-side forward motion

	// PPS output registers
	RPOR0bits.RP65R = 16; // Assigns OC1 to RD1 (Left Backward)
	RPOR1bits.RP66R = 17; // Assigns OC2 to RD2 (Left Forward)
	RPOR1bits.RP67R = 18; // Assigns OC3 to RD3 (Right Backward)
	RPOR2bits.RP68R = 19; // Assigns OC4 to RD4 (Right Forward)

	// Cleaning Control registers before initialization
	OC1CON1 = 0;
	OC1CON2 = 0;
	OC2CON1 = 0;
	OC2CON2 = 0;
	OC3CON1 = 0;
	OC3CON2 = 0;
	OC4CON1 = 0;
	OC4CON2 = 0;

	// Initialize PWM-period = (Fcy / (Fpwm * prescaler)) -1
	unsigned int pwm_period = F_CY / (F_PWM * PRESCALER) - 1;

	// Select clock source
	OC1CON1bits.OCTSEL = 7;
	OC2CON1bits.OCTSEL = 7;
	OC3CON1bits.OCTSEL = 7;
	OC4CON1bits.OCTSEL = 7;

	// Enable self-synchronization
	OC1CON2bits.SYNCSEL = 0x1F;
	OC2CON2bits.SYNCSEL = 0x1F;
	OC3CON2bits.SYNCSEL = 0x1F;
	OC4CON2bits.SYNCSEL = 0x1F;

	// Load pwm_period value into the period register
	OC1RS = pwm_period;
	OC2RS = pwm_period;
	OC3RS = pwm_period;
	OC4RS = pwm_period;

	// Set initial speed to zero (start duty cycle)
	OC1R = 0;
	OC2R = 0;
	OC3R = 0;
	OC4R = 0;

	// Turn on the module
	OC1CON1bits.OCM = 6;
	OC2CON1bits.OCM = 6;
	OC3CON1bits.OCM = 6;
	OC4CON1bits.OCM = 6;
}

/**
 * @brief Converts a percentage into a hardware register duty cycle value.
 *
 * @param percent The desired duty cycle ranging from 0 to 100.
 * @param reg The period register boundary (OCxRS) to scale against.
 * @return int The calculated hardware integer to be written to OCxR.
 */
int pwm_set_duty_cycle(int percent, unsigned int reg) {

	// Rimosso il blocco "percent <= 40"
	if (percent < 0) {
		percent = 0; // Sicurezza per evitare valori negativi
	}

	if (percent > 100) {
		percent = 100;
	}

	return ((unsigned long)reg * percent) / 100;
}

/**
 * @brief Clamps an input value to the absolute limits of the motor saturation [-100, 100].
 *
 * @param value The raw kinematic speed demand.
 * @return int The bounded speed value.
 */
int clamp(int value) {
	if (value > 100)
		value = 100;
	if (value < -100)
		value = -100;
	return value;
}

/**
 * @brief Computes differential kinematics and updates H-Bridge duty cycles.
 *
 * Converts a target forward velocity and angular yaw rate into independent 
 * left and right track speeds. It applies threshold clamps and routes 
 * the resulting magnitudes to the appropriate forward or backward OC module.
 *
 * @param v Target linear velocity (-100 to 100).
 * @param w Target angular yaw rate (-100 to 100).
 */
void execute_motion(int v, int w) {

	// === Left-right power computation ===

	int left_pwm = v - w;
	int right_pwm = v + w;

	// Clamp between -100 and 100

	left_pwm = clamp(left_pwm);
	right_pwm = clamp(right_pwm);

	// === LEFT WHEELS ACTUATION ===

	if (left_pwm > 0) {
		// Forward -> RD2 active, RD1 = 0
		OC1R = 0;
		OC2R = pwm_set_duty_cycle(left_pwm, OC2RS);
	} else if (left_pwm < 0) {
		// Backward -> RD1 active, RD2 = 0
		OC1R = pwm_set_duty_cycle(-left_pwm, OC1RS);
		OC2R = 0;
	} else {
		// Stop
		OC1R = 0;
		OC2R = 0;
	}

	// === RIGHT WHEELS ACTUATION ===

	if (right_pwm > 0) {
		// Forward -> RD4 active, RD3 = 0
		OC3R = 0;
		OC4R = pwm_set_duty_cycle(right_pwm, OC4RS);
	} else if (right_pwm < 0) {
		// Backward -> RD3 active, RD4 = 0
		OC3R = pwm_set_duty_cycle(-right_pwm, OC3RS);
		OC4R = 0;
	} else {
		// Stop
		OC3R = 0;
		OC4R = 0;
	}
}