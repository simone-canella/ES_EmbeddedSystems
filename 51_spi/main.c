/**
 * @file main.c
 * @author simon
 * @brief Main function: 10 Hz Magnetometer X-Axis Telemetry Streaming via SPI.
 */

#include "../library/spi.c"
#include "../library/spi.h"
#include "../library/timer.c"
#include "../library/timer.h"
#include "../library/uart.c"
#include "../library/uart.h"
#include <stdio.h>
#include <xc.h>

int main() {

	// SETUP PIN: Configure all pins as digital
	ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

	// TIMER SETUP
	tmr_setup_period(TIMER1, 100);

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
	tmr_wait_ms(TIMER2, 3);

	// Sleep -> Active (write 0x00 at 0x4C)
	LATDbits.LATD6 = 0; // Select Magnetometer (CS3 on RD6)
	spi1_write(0x4C);	// Target Operation Mode Register (Write mode, MSB=0)
	spi1_write(0x00);	// Load OpMode bits = 00b (Selects Normal Mode)
	LATDbits.LATD6 = 1; // Deselect Magnetometer to close transaction

	int missed_deadline = 0;

	// EXECUTION-LOOP at 10Hz (100ms)
	while (1) {
		// -------------------------------------------------------------------------
		// ACQUIRE X-AXIS & SEND TO UART (read procedure)
		// -------------------------------------------------------------------------
		unsigned int lsb_byte = 0;
		unsigned int msb_byte = 0;
		unsigned int trash = 0;

		// the x-axis is a 13bit data split in register 0x42 and 0x43
		LATDbits.LATD6 = 0;				 // Select Magnetometer (CS3 on RD6)
		trash = spi1_write(0x42 | 0x80); // Send read adrress header with MSB set high
		lsb_byte = spi1_write(0x00);	 // Extract LSB byte from 0x42
		msb_byte = spi1_write(0x00);	 // Extract MSB byte from 0x43
		LATDbits.LATD6 = 1;				 // Deselect Magnetometer to close transaction

		// Compact data (shift and mask)
		int data = (msb_byte << 8) | (lsb_byte & 0xF8);
		data = data >> 3;

		// Format the value into an ASCII string and transmit via non-blocking UART
		char message[16];
		sprintf(message, "$MAGX=%f*\n", data);

		uart1_write_string(message);

		if (tmr_wait_period(TIMER1) == 1) {
			missed_deadline++;
		}
	}

	return 0;
}
