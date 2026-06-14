/**
 * @file main.c
 * @author simon
 * @brief This program controls LED1 using non-blocking edge detection on Button T2. 
 */

#include <stdbool.h>
#include <xc.h>

int main() {

	// SETUP: make all pins digital
	ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

	// Configure the pin directions (0 = output, 1 = input)
	TRISAbits.TRISA0 = 0; // LED1(RA0)
	TRISEbits.TRISE8 = 1; // Button1 (RE8)

	// state button tracker
	bool buttonPressed = 0;

	while (1) {

		if (!PORTEbits.RE8 && !buttonPressed) { // check if the button is pressed
			buttonPressed = 1;
		}

		if (PORTEbits.RE8 && buttonPressed) { // check if the button is not more pressed
			LATAbits.LATA0 = !LATAbits.LATA0; // change the LED1 state
			buttonPressed = 0;
		}
	}

	return 0;
}
