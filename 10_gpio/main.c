/**
 * @file main.c
 * @brief This program controls LED1 based on the real-time state of Button T2.
 * @author Simone Canella
 * @version 1.0.0
 */

#include <xc.h>

int main(void) {
	// SETUP: make all pins digital
	ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

	// Configure pin direction (0 = output, 1 = input)
	TRISAbits.TRISA0 = 0; // LED1 (RA0)
	TRISEbits.TRISE8 = 1; // Button1 (RE8)

	while (1) {
		// turn the led on when the button is pressed
		if (PORTEbits.RE8) { // pressed state is 0V
			LATAbits.LATA0 = 1; // turn LED1 on
		} else {
            LATAbits.LATA0 = 0; // turn LED1 off
		}
	}

	return 0;
}