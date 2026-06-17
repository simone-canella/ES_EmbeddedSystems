/**
 * @file BOSCH_BMX055.c
 * @brief SPI driver implementation for the Bosch BMX055 IMU.
 *
 * This file contains the initialization sequences, power management routines, 
 * and data extraction functions required to interface with the Accelerometer, 
 * Gyroscope, and Magnetometer components of the BMX055 sensor over SPI.
 */

#include "BOSCH_BMX055.h"
#include <xc.h>
#include "timer.h"
#include "spi.h"

/**
 * @brief Global volatile buffers for temporarily storing raw SPI byte transactions.
 */
volatile unsigned int DATA_LSB, DATA_MSB;

/**
 * @brief Initializes the BMX055 sensor and configures the SPI bus parameters.
 *
 * * Sets Chip Select (CS) lines for the Accelerometer, Gyroscope, and Magnetometer to high (inactive).
 * * Temporarily disables the SPI module to configure protocol settings matching the BMX055 requirements.
 * * Configures the SPI clock polarity to idle LOW (CKP = 0) and sets data changes to occur on transition from active to idle (CKE = 1).
 * * Sets the accelerometer's initial bandwidth to 1000 Hz by writing value 15 to register 0x10.
 */
void BMX055_setup(){
    ACC_CS = 1;
    GYR_CS = 1;
    MAG_CS = 1;

    // Bosch BMX055 SPI protocol require idle LOW 
    SPI1STATbits.SPIEN = 0;     // disable SPI to change settings

    SPI1CON1bits.CKE = 1; // Output data changes on transition from active to idle
    SPI1CON1bits.CKP = 0; // Idle state for clock is a low level

    SPI1STATbits.SPIEN = 1;     // re-enable SPI

    tmr_wait_ms(TIMER1, 100);

    // Set Initial Bandwidth to 1000 Hz (Register 0x10, Value 15)
    ACC_CS = 0;
    spi1_write(0x10 & 0x7F); // MSB = 0 for write
    spi1_write(15); 
    ACC_CS = 1;

    tmr_wait_ms(TIMER1, 100);
}

/**
 * @brief Executes the wake-up sequence for the BMX055 magnetometer.
 *
 * * Forces the magnetometer interface to switch from I2C to SPI by toggling the MAG_CS pin.
 * * Transitions the magnetometer from Suspend Mode to Sleep Mode by writing 0x01 to register 0x4B.
 * * Transitions the magnetometer from Sleep Mode to Active Mode by writing 0x00 to register 0x4C.
 * * Utilizes TIMER1 delays between transmissions to allow for power mode switching times.
 */
void BMX055_mag_wakeup(){
    
    // passaggio forzato da I2C ad SPI (Gemini)
    MAG_CS = 0;
    tmr_wait_ms(TIMER1, 2);
    MAG_CS = 1;
    tmr_wait_ms(TIMER1, 2);

    // 1. Suspend -> Sleep Mode (write 0x01 at 0x4B)
    MAG_CS = 0;
    spi1_write(0x4B); 
    spi1_write(0x01);
    MAG_CS = 1;              // End first transmission

    tmr_wait_ms(TIMER1, 50); // sleep time for power mode switching

    // 2. Sleep Mode -> Active Mode (write 0x00 at 0x4C)
    MAG_CS = 0;              // Start new transmission
    spi1_write(0x4C); 
    spi1_write(0x00);        
    MAG_CS = 1;

    tmr_wait_ms(TIMER1, 50); 
}

/**
 * @brief Reads two bytes via SPI and formats them as a 12-bit signed integer.
 *
 * * Masks the lower 4 bits of the LSB with 0b11110000 to isolate the valid data.
 * * Shifts the MSB left by 8 bits and combines it with the LSB.
 * * Divides the combined short by 16 to properly align the 12-bit value.
 * @return int The formatted 12-bit signed data value.
 */
int format12bit(){
    DATA_LSB = spi1_write(0x00);
    DATA_MSB = spi1_write(0x00);
    DATA_LSB = DATA_LSB & 0b11110000;
    DATA_MSB = DATA_MSB << 8;
    short combined = (short)(DATA_MSB | DATA_LSB);
    return (int)(combined / 16);
}

/**
 * @brief Reads two bytes via SPI and formats them as a 13-bit signed integer.
 *
 * * Masks the lower 3 bits of the LSB with 0b11111000 to isolate the valid data.
 * * Shifts the MSB left by 8 bits and combines it with the LSB.
 * * Divides the combined short by 8 to properly align the 13-bit value.
 * @return int The formatted 13-bit signed data value.
 */
int format13bit(){
    DATA_LSB = spi1_write(0x00);
    DATA_MSB = spi1_write(0x00);
    DATA_LSB = DATA_LSB & 0b11111000;
    DATA_MSB = DATA_MSB << 8;
    short combined = (short)(DATA_MSB | DATA_LSB);
    return (int)(combined / 8);
}

/**
 * @brief Reads two bytes via SPI and formats them as a 15-bit signed integer.
 *
 * * Masks the lowest bit of the LSB with 0b11111110 to isolate the valid data.
 * * Shifts the MSB left by 8 bits and combines it with the LSB.
 * * Divides the combined short by 2 to properly align the 15-bit value.
 * @return int The formatted 15-bit signed data value.
 */
int format15bit(){
    DATA_LSB = spi1_write(0x00);
    DATA_MSB = spi1_write(0x00);
    DATA_LSB = DATA_LSB & 0b11111110;
    DATA_MSB = DATA_MSB << 8;
    short combined = (short)(DATA_MSB | DATA_LSB);
    return (int)(combined / 2);
}

/**
 * @brief Reads two bytes via SPI and formats them as a standard 16-bit signed integer.
 *
 * * Shifts the MSB left by 8 bits and performs a bitwise OR with the LSB.
 * @return int The combined 16-bit signed data value.
 */
int format16bit(){
    DATA_LSB = spi1_write(0x00);
    DATA_MSB = spi1_write(0x00);
    DATA_MSB = DATA_MSB << 8;
    return (int)((short)(DATA_MSB | DATA_LSB));
}

/**
 * @brief Reads a full frame of multi-axis data from the specified IMU sensor.
 *
 * * Dispatches SPI reads depending on the requested sensor macro (`ACC`, `GYR`, `MAG`).
 * * Sets the MSB of the register address to 1 (0x80) to indicate a read operation.
 * * Accelerometer (ACC): Reads from register 0x02 and formats X, Y, and Z axes as 12-bit integers.
 * * Gyroscope (GYR): Reads from register 0x02 and formats X, Y, and Z axes as 16-bit integers.
 * * Magnetometer (MAG): Reads from register 0x42, formats X and Y axes as 13-bit integers, and the Z axis as a 15-bit integer.
 *
 * @param sensor The target sensor peripheral to read (`ACC`, `GYR`, or `MAG`).
 * @return IMUdata A structure containing the extracted x, y, and z axis measurements.
 */
IMUdata imuRead(int sensor){
    IMUdata data;

    switch (sensor){
        case ACC:
            ACC_CS = 0;
            spi1_write(0x02| 0x80); // MSB = 1 per leggere
            data.x = format12bit();
            data.y = format12bit();
            data.z = format12bit();
            ACC_CS = 1;
            break;

        case GYR:
            GYR_CS = 0;
            spi1_write(0x02| 0x80); // MSB = 1 per leggere
            data.x = format16bit(); 
            data.y = format16bit(); 
            data.z = format16bit(); 
            GYR_CS = 1;
            break;

        case MAG:
            MAG_CS = 0;
            spi1_write(0x42| 0x80); // MSB = 1 per leggere
            data.x = format13bit();
            data.y = format13bit();
            data.z = format15bit();
            MAG_CS = 1;
            break;
    }

    return data;
}