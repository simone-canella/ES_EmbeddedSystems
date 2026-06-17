/**
 * @file spi.c
 * @brief SPI interface driver and hardware configuration.
 *
 * This module provides the hardware setup and primitive transmit/receive 
 * functions for the SPI1 peripheral, specifically routed to the MikroBus1 
 * header footprint.
 */

#include "timer.h"

/**
 * @brief Configures the SPI1 peripheral pins, control registers, and default chip selects.
 *
 * * Maps the MISO line to input pin RA1 (RPI17).
 * * Maps the MOSI line to output pin RF13 (RP109).
 * * Maps the SCK (Serial Clock) line to output pin RF12 (RP108).
 * * Initializes three independent Chip Select (CS) pins (RB3, RB4, RD6) as digital outputs idling HIGH (inactive).
 * * Configures the SPI core for Master Mode, 8-bit data width, and sets prescalers to control the SCK frequency.
 * * Enables the SPI1 module.
 */
void spi1_mikrobus1_setup(){

        // MISO
    TRISAbits.TRISA1 = 1;           // RA1 -> RPI17
    RPINR20bits.SDI1R = 0b0010001;  // (SDI1) -> RPI17

        // MOSI
    TRISFbits.TRISF13 = 0;          // RF13 -> RP109
    RPOR12bits.RP109R = 0b000101;   // (SDO1) -> RF13

        // SCK  
    TRISFbits.TRISF12 = 0;          // RF12 -> RP108
    RPOR11bits.RP108R = 0b000110;   // SCK1  -> RF12

        // CS - idle high first
    LATBbits.LATB3 = 1;
    LATBbits.LATB4 = 1;
    LATDbits.LATD6 = 1;

        // set pins as outputs
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISDbits.TRISD6 = 0;

        // protocol SPI
    SPI1CON1bits.MSTEN = 1;     // master mode
    SPI1CON1bits.MODE16 = 0;    // 8-bit mode
    SPI1CON1bits.PPRE = 3;      // 1:1 primary prescaler
    SPI1CON1bits.SPRE = 3;      // 5:1 secondary prescaler

    SPI1STATbits.SPIEN = 1;
}

/**
 * @brief Transmits and receives a single byte over the SPI bus.
 *
 * This is a blocking, synchronous hardware call. SPI communication is full-duplex, 
 * meaning a byte is always received simultaneously as a byte is transmitted.
 *
 * @param data The 8-bit data payload to transmit.
 * @return unsigned int The 8-bit data payload received from the slave device.
 */
unsigned int spi1_write(unsigned int data) {
    while (SPI1STATbits.SPITBF == 1); // Wait until TX buffer is empty
    SPI1BUF = data;                   // Send the byte
    while (SPI1STATbits.SPIRBF == 0); // Wait until RX buffer is full
    return SPI1BUF;                   // Return the received byte
}