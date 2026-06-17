#include "timer.h"

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

        // CS
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISDbits.TRISD6  = 0;

    LATBbits.LATB3 = 1;
    LATBbits.LATB4 = 1;
    LATDbits.LATD6 = 1;

        // protocol SPI
    SPI1CON1bits.MSTEN = 1;     // master mode
    SPI1CON1bits.MODE16 = 0;    // 8-bit mode

    // max clock frequency: 10MHz (p.141/172 imu_datasheet)
    // 72MHz/(PPRE * SPRE) = 72MHz/8 = 9MHz < 10MHz
    SPI1CON1bits.PPRE = 0b11;   // 8:1 primary prescaler
    SPI1CON1bits.SPRE = 0b000;  // 1:1 secondary prescaler

    SPI1STATbits.SPIEN = 1;
}

unsigned int spi1_write(unsigned int data) {
    while (SPI1STATbits.SPITBF == 1); // Wait until TX buffer is empty
    SPI1BUF = data;                   // Send the byte
    while (SPI1STATbits.SPIRBF == 0); // Wait until RX buffer is full
    return SPI1BUF;                   // Return the received byte
}