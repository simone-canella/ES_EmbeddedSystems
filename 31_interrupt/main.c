/**
 * @file main.c
 * @author simon
* @brief Task 2: Polling LED1 heartbeat with asynchronous button interrupt on LED2
 */

#include <xc.h>
#include "../library/timer.c"
#include "../library/timer.h"

// Function Prototypes
void interrupt_setup(void);

int main() {
	// PIN SETUP: configure all pins as digital
	ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

	// Configure pin direction (0 = output, 1 = input)
	TRISAbits.TRISA0 = 0; // LED1 (RA0)
	TRISGbits.TRISG9 = 0; // LED2 (RG9)
	TRISEbits.TRISE8 = 1; // Button2 (RE8)

	// TIMER SETUP: configure the timer's period
	tmr_setup_period(TIMER1, 500); // toggle at 1Hz

    interrupt_setup();

	// EXECUTION LOOP
	while (1) {
        // toggle the LED1 at 1Hz
		if (tmr_wait_period(TIMER1)) {
			LATAbits.LATA0 = !LATAbits.LATA0;
		}
	}

	return 0;
}

/** 
 * @brief Configure the interrupt's: priority (IPC), flag(IFS), enable(IEC), pheriphera-pin-select
*/
void interrupt_setup() {
    //PPS: Map External Interrupt 1 (INT1) to pin RPI88 (RE8)
    RPINR0bits.INT1R = 0b1011000; 

    // INT1 Setup (Button2)
	IPC5bits.INT1IP = 1; // set priority
	IFS1bits.INT1IF = 0; // clear the flag
	IEC1bits.INT1IE = 1; // enable interrupt

    // TIMER2
    IPC1bits.T2IP = 2; // set priority
    IFS0bits.T2IF = 0; // clear the flag
    IEC0bits.T2IE = 0; // keep disabled until button press

    // Global Interrupt Switch
    INTCON2bits.GIE = 1; // Unlock system-wide interrupt vector execution
}

/** 
 * @brief Button interrupt function
 * 
 * @details When the button is pressed it starts a timer of 10ms for avoiding debouncing, then calls the timer interrupt function
*/
void __attribute__((__interrupt__, __auto_psv__)) _INT1Interrupt(void) {
    IFS1bits.INT1IF = 0; // clear the flag

    // when the button is pressed starts a 10ms TIMER2 to avoid debouncing errors
    IEC0bits.T2IE = 1; // enable
    tmr_setup_period(TIMER2, 10); 
}

/** 
 * @brief Timer interrupt function is activated one time for toggleling the LED2
 * 
 * @details After the debouncing procedure, Toggle the LED2
*/
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void) {
    IFS0bits.T2IF = 0; // clear the timer2 flag
    IEC0bits.T2IE = 0; // disable TIMER2 interrupt
    T2CONbits.TON = 0; // turn off TIMER2

    // toggle the LED2
    LATGbits.LATG9 = !LATGbits.LATG9;
}


