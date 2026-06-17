/**
 * @file main.c
 * @author simon
 * @date 2026-06-17
 * @brief Main function: Button-triggered differential motor control loop.
 */

#include "../library/pwm.c"
#include "../library/pwm.h"
#include "../library/timer.c"
#include "../library/timer.h"
#include <stdio.h>
#include <xc.h>

#define TARGET_SPEED 60 // Target linear speed percentage [40 to 100]
#define TARGET_YAW 0	// Target angular yaw rate percentage [-100 to 100]

// FUNCTION PROTOTYPE
void interrupt_setup();

// GLOBAL VARIABLE
volatile int motor_active = 0; // 0 = stopped, 1 = moving

int main() {
	// SETUP PIN: Configure all pins as digital
	ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

	// SETUP TIMER
	tmr_setup_period(TIMER1, 100);

    // INTERRUPT SETUP
    interrupt_setup();

	// PWM  SETUP
	pwm_setup();

	int missed_deadline = 0;

	// EXECUTION-LOOP
	while (1) {
		if (motor_active) {
			// Drive the wheels using your differential kinematic engine
			execute_motion(TARGET_SPEED, TARGET_YAW);
		} else {
			// Force a full hardware bridge stop state if disabled
			execute_motion(0, 0);
		}

		// Enforce the execution frame duration alignment
		if (tmr_wait_period(TIMER1) == 1) {
			missed_deadline++;
		}
	}

	return 0;
}

// --------------------
// INTERRUPT FUNCTIONS
// --------------------
/**
 * @brief Configure the interrupt's: priority (IPC), flag(IFS), enable(IEC),
 * pheriphera-pin-select
 */
void interrupt_setup() {
	// PPS: Map External Interrupt 1 (INT1) to pin RPI88 (Button2 RE8)
	RPINR0bits.INT1R = 0b1011000;

    // Configure INT1 to trigger explicitly on falling edge (button press) 
	INTCON2bits.INT1EP = 1;

	// INT1 Setup (Button2)
	IPC5bits.INT1IP = 1; // set priority
	IFS1bits.INT1IF = 0; // clear the flag
	IEC1bits.INT1IE = 1; // enable interrupt

	// TIMER3
	IPC2bits.T3IP = 2; // set priority
	IFS0bits.T3IF = 0; // clear the flag
	IEC0bits.T3IE = 0; // keep disabled until button press
}

/**
 * @brief Button2 interrupt function
 *
 * @details When the button is pressed it starts a timer of 10ms for avoiding
 * debouncing, then calls the timer interrupt function
 */
void __attribute__((__interrupt__, __auto_psv__)) _INT1Interrupt(void) {
	IFS1bits.INT1IF = 0; // clear the flag

	// when the button is pressed starts a 10ms TIMER3 to avoid debouncing errors
	IEC0bits.T3IE = 1; // enable
	tmr_setup_period(TIMER3, 10);
}

/**
 * @brief Timer3 interrupt function is activated one time for sending the
 * message
 *
 * @details After the debouncing procedure, sending the current number of chars
 * received from the UART1
 */
void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void) {
	IFS0bits.T3IF = 0; // clear the TIMER3 flag
	IEC0bits.T3IE = 0; // disable TIMER3 interrupt
	T3CONbits.TON = 0; // turn off TIMER3

	motor_active = !motor_active;
}
