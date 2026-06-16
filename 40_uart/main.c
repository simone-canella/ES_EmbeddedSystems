/**
 * @file main.c
 * @author simon
 * @date 2026-06-15
 * @brief Main function
 */

#include "../library/timer.c"
#include "../library/timer.h"
#include <xc.h>

#define FCY 72000000UL
#define BAUDRATE 9600

int main() {
	// PIN SETUP: configure all pins as digital
	ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

	// UART SETUP
	uart_setup();

	char data;

	// EXECUTION-LOOP
	while (1) {
		data = uart1_read_char();

		uart1_write_char(data);
	}
	return 0;
}

/**
 * @brief Configures UART1 module parameters, I/O cell directionality, and PPS
 * lines.
 */
void uart_setup() {
	// PPS Mapping
	RPINR18bits.U1RXR =
		75; // Map U1RX function to remappable input RPI75 (RD11)
	RPOR0bits.RP64R =
		1; // Map U1TX function (code 1) to remappable output RP64 (RD0)

	// Configure the baud rate clock divider (9600 at 72MHz)
	U1BRG = (FCY / (16 * BAUDRATE)) - 1;

	// Configure Data Framing Parameters
	U1MODEbits.PDSEL = 0; // 8-bit data, no parity
	U1MODEbits.STSEL = 0; // 1 stop bit

	// Hardware Activation Sequencing
	U1MODEbits.UARTEN = 1; // Enable master UART module clock
	U1STAbits.UTXEN = 1;   // Enable transmission pin line drivers
}

/**
 * @brief Blocks execution until a byte is available in the receiver FIFO
 * buffer, handling overruns.
 * @return char The oldest ASCII byte extracted from the FIFO queue.
 */
char uart1_read_char() {
	// Trap and resolve buffer overrun errors to prevent peripheral freeze conditions
	if (U1STAbits.OERR == 1) {
		U1STAbits.OERR = 0; // reset receiver if it is blocked
	}

	while (U1STAbits.URXDA == 0)
		;			// Wait until data is avabaile in RX buffer
	return U1RXREG; // return the received character
}

/**
 * @brief Transmits a single character character out through the serial bus
 * line.
 * @param c The character byte scheduled for transmission.
 */
void uart1_write_char(char c) {
	while (U1STAbits.UTXBF == 1)
		; // wait if the FIFO queue is full

	U1TXREG = c;
}