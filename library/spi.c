#include "spi.h"

/**
 * @brief Configures SPI!1 module parameters, peripheral pins, control
 * registers, and PPS lines.
 *
 * @details
 * * Maps the MISO line to input pin RA1 (RPI17).
 * * Maps the MOSI line to output pin RF13 (RP109).
 * * Maps the SCK (Serial Clock) line to output pin RF12 (RP108).
 * *Initializes three independent Chip Select (CS) pins (RB3, RB4, RD6) as
 * digital outputs idling HIGH (inactive).
 * * Configures the SPI core for Master Mode, 8-bit data width, and sets
 * prescalers to control the SCK frequency.
 * * Enables the SPI1 module.
 */
void spi1_mikrobus2_setup(void) {
	// MISO Configuration
	TRISAbits.TRISA1 = 1;	//  Set RA1 as digital input
	RPINR20bits.SDI1R = 17; // Remappable Input function RA1 -> RPI17

	// MOSI Configuration
	TRISFbits.TRISF13 = 0; // Set RF13 as digital output
	RPOR12bits.RP109R = 5; // Remappable Output function RF13 -> RP109

	// SCK Configuration
	TRISFbits.TRISF12 = 0; // Set RF12 as digital output
	RPOR11bits.RP108R = 6; // Remappable Output function code RF12 -> RP108

	// Chip Select Configuration
	LATBbits.LATB3 = 1;
	LATBbits.LATB4 = 1;
	LATDbits.LATD6 = 1;

	// Confire Pins as Output
	TRISBbits.TRISB3 = 0; // Accelerometer (RB3)
	TRISBbits.TRISB4 = 0; // Gyroscope (RB4)
	TRISDbits.TRISD6 = 0; // Magnetometer (RD6)

	// Confire SPI Protocol
	SPI1CON1bits.MSTEN = 1;	 // Master Mode
	SPI1CON1bits.MODE16 = 0; // 8-bit Mode
	SPI1CON1bits.PPRE = 3;	 // 1:1 primary prescaler
	SPI1CON1bits.SPRE = 3;	 // 5:1 primary prescaler

	// Configure Clock Polarity
	SPI1CON1bits.CKP = 1; // Clock idle state is logical high

	// Enable SPI
	SPI1STATbits.SPIEN = 1; // enable SPI
}

/**
 * @brief Transmits and receives a single byte over the SPI bus.
 *
 * This is a blocking, synchronous hardware call. SPI communication is
 * full-duplex, meaning a byte is always received simultaneously as a byte is
 * transmitted.
 *
 * @param data The 8-bit data payload to transmit.
 * @return unsigned int The 8-bit data payload received from the slave device.
 */
unsigned int spi1_write(unsigned int data) {
	// Wait until TX buffer is empty
	while (SPI1STATbits.SPITBF == 1) {
		;
	}

	SPI1BUF = data;

	// Wait until RX buffer is full (data is fully arrived from the slave)
	while (SPI1STATbits.SPIRBF == 0) {
		;
	}

	return SPI1BUF; // return the received byte
}
