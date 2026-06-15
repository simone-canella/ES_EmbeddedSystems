/**
 * @file main.c
 * @author simon
 * @brief Task 1: Dual frequency blinking using Polling (LD1) and Interrupts
 * (LD2)
 */

#include "../library/timer.c"
#include "../library/timer.h"
#include <xc.h>

// Function Prototype
void interrupt_setup_timer2(void);

int main() {
	// PIN SETUP: configure all pins as digital
	ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

	// Configure pin direction (0 = output, 1 = input)
	TRISAbits.TRISA0 = 0; // LED1 (RA0)
	TRISGbits.TRISG9 = 0; // LED2 (RG9)

	// TIMER SETUP: configure the timer's period
	tmr_setup_period(TIMER1, 500); // toggle at 1Hz
	tmr_setup_period(TIMER2, 250); // toggle at 2Hz

	// INTERRUPT SETUP: configure the interrupt
	interrupt_setup();

	while (1) {
		// toggle LED1 at 1Hz
		if (tmr_wait_period(TIMER1)) {
			LATAbits.LATA0 = !LATAbits.LATA0;
		}
	}

	return 0;
}

/** 
 * @brief Configure the interrupt's: priority (IPC), flag(IFS), enable(IEC)
*/
void interrupt_setup() {
	IPC1bits.T2IP = 1; // set interrupt priority
	IFS0bits.T2IF = 0; // clear the flag
	IEC0bits.T2IE = 1; // enable the interrupt
}

/** 
 * @brief Timer interrupt function
 * 
 * @details Toggle the LED2
*/
void __attribute__((__interrupt__, __auto_psv__)) T2Interrupt(void) {
	IFS0bits.T2IF = 0; // clear the flag
    
	// toggle LED2 at 2Hz
	LATGbits.LATG9 = !LATGbits.LATG9;
}
