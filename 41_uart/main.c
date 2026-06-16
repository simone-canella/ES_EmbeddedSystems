/**
 * @file main.c
 * @author simon
 * @brief Main function: Interrupt-driven non-blocking UART basic character echo.
 */

#include "../library/timer.c"
#include "../library/timer.h"
#include "../library/uart.c"
#include "../library/uart.h"
#include <xc.h>

int main() {
	// PIN SETUP: configure all pins as digital
	ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

	// UART SETUP
	uart1_mikrobus2_setup();

	char data;

	// EXECUTION-LOOP
	while (1) {
		// Fetch data (non-blocking)
		data = uart1_read_char();

		// If data is valid echo it back
		if (data != -1) {
			// Create a small local string to pass to the string function
			char echo_str[2];
			echo_str[0] = data;
			echo_str[1] = '\0'; // Null terminator for safety
			uart1_write_string(echo_str);
		}
	}
	return 0;
}
