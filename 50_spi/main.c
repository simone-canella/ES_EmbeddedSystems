/**
 * @file main.c
 * @author simon
 * @brief Main function: SPI Magnetometer identification sequence. */

#include "../library/spi.c"
#include "../library/spi.h"
#include "../library/timer.c"
#include "../library/timer.h"
#include "../library/uart.c"
#include "../library/uart.h"
#include <stdio.h>
#include <xc.h>

int main() {
	// PIN SETUP: configure all pins as digital
	ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

	// UART SETUP
	uart1_mikrobus2_setup();

	// SPI SETUP
	spi1_mikrobus2_setup();

	// -------------------------------------------------------------------------
	// CHANGE MAGNETOMETER MODE (write procedure)
	// -------------------------------------------------------------------------
	// Suspend -> Sleep (write 0x01 at 0x4B)
	LATDbits.LATD6 = 0; // Select Magnetometer (CS3 on RD6)
	spi1_write(0x4B);	// Target Power Control Register (Write mode, MSB=0)
	spi1_write(0x01);	// Load Power Control Bit = 1 (Enables Sleep State)
	LATDbits.LATD6 = 1; // Deselect Magnetometer to close the SPI block

	// Waiting 3ms stabilization
	tmr_wait_ms(TIMER1, 3);

	// Sleep -> Active (write 0x00 at 0x4C)
	LATDbits.LATD6 = 0; // Select Magnetometer (CS3 on RD6)
	spi1_write(0x4C);	// Target Operation Mode Register (Write mode, MSB=0)
	spi1_write(0x00);	// Load OpMode bits = 00b (Selects Normal Mode)
	LATDbits.LATD6 = 1; // Deselect Magnetometer to close transaction

	// -------------------------------------------------------------------------
	// ACQUIRE CHIP ID & SEND TO UART (read procedure)
	// -------------------------------------------------------------------------
	unsigned int chip_id = 0;
	unsigned int trash = 0;

	LATDbits.LATD6 = 0;				 // Select Magnetometer (CS3 on RD6)
	trash = spi1_write(0x40 | 0x80); // Send read address header with MSB set high
	chip_id = spi1_write(0x00);		 // Send dummy byte to receive the sensor's register data
	LATDbits.LATD6 = 1;				 // Deselect Magnetometer to close transaction

	// Format the value into an ASCII string and transmit via non-blocking UART
	char uart_buffer[32];
	sprintf(uart_buffer, "Chip ID = 0x%X\n", chip_id);
	uart1_write_string(uart_buffer);

	while (1) {
		;
	}

	return 0;
}
