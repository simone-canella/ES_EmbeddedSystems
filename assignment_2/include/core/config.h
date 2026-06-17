/**
 * @file config.h
 * @brief Central configuration and pin mapping definition header for the robot buggy.
 * @author Simone Canella, Samuele Isaia, Riccardo Parotelli
 * @version 1.0.0
 *
 * @details This file centralizes all global constants, peripheral configuration parameters,
 * software settings, and Peripheral Pin Select (PPS) hardware mappings for the dsPIC microcontroller.
 * It ensures consistency across all compilation modules.
 */

#ifndef CONFIG_H
#define CONFIG_H

// --- Standard C Libraries ---
#include <xc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/** --------------------------------------------------
 * @name System Constants
 * @{
 */
#define FCY 72000000UL          /**< Instruction Frequency (72 MHz). */
#define RAD_TO_DEG 57.29577951f /**< Conversion factor from Radians to Degrees (180.0f / PI). */
/** @} */

/** --------------------------------------------------
 * @name UART Configuration
 * @{
 */
#define BAUDRATE 9600                     /**< Centralized Baudrate for PC data logging/command interface. */
#define BRGVAL ((FCY / BAUDRATE) / 4) - 1 /**< Baud Rate Generator value calculated for High-Speed mode (BRGH = 1). */

// UART1 PPS (Peripheral Pin Select)
#define UART1_RX_RPIN 75    /**< Remapped UART1 RX signal pin ID: RPI75 (RD11). */
#define UART1_TX_RPIN 64    /**< Remapped UART1 TX signal pin ID: RP64 (RD0). */
/** @} */

/** --------------------------------------------------
 * @name SPI1 Configuration
 * @{
 */
#define SPI1_PPRE 0b11      /**< 8:1 primary prescaler configuration bits. */
#define SPI1_SPRE 0b000     /**< 1:1 secondary prescaler configuration bits. */

// SPI1 PPS (Peripheral Pin Select)
#define SPI1_MISO_RPIN 17   /**< Remapped SPI1 MISO signal pin ID: RPI17 (RA1). */
#define SPI1_MOSI_RPIN 109  /**< Remapped SPI1 MOSI signal pin ID: RP109 (RF13). */
#define SPI1_SCK_RPIN  108  /**< Remapped SPI1 SCK signal pin ID: RP108 (RF12). */
/** @} */

/** --------------------------------------------------
 * @name Sensor Chip Selects (BOSCH BMX055 IMU)
 * @{
 */
// Accelerometer (RB3)
#define ACC_CS_LAT  LATBbits.LATB3      /**< Accelerometer Chip Select Latch Register */
#define ACC_CS_TRIS TRISBbits.TRISB3    /**< Accelerometer Chip Select Direction Register */
#define ACC_CHIP_ID 0xFA                /**< Accelerometer hardware ID */

// Gyroscope (RB4)
#define GYR_CS_LAT  LATBbits.LATB4      /**< Gyroscope Chip Select Latch Register */
#define GYR_CS_TRIS TRISBbits.TRISB4    /**< Gyroscope Chip Select Direction Register */
#define GYR_CHIP_ID 0x0F                /**< Gyroscope hardware ID */

// Magnetometer (RD6)
#define MAG_CS_LAT  LATDbits.LATD6      /**< Magnetometer Chip Select Latch Register */
#define MAG_CS_TRIS TRISDbits.TRISD6    /**< Magnetometer Chip Select Direction Register */
#define MAG_CHIP_ID 0x32                /**< Magnetometer hardware ID */
/** @} */

/** --------------------------------------------------
 * @name Magnetometer Calibration Offsets
 * @{
 */
#define MAG_OFFSET_X 0.0f /**< X-axis hard-iron calibration offset for the magnetometer. */
#define MAG_OFFSET_Y 0.0f /**< Y-axis hard-iron calibration offset for the magnetometer. */
#define MAG_OFFSET_Z 0.0f /**< Z-axis hard-iron calibration offset for the magnetometer. */
/** @} */

/** --------------------------------------------------
 * @name GPIO Mappings: LEDs, Buttons, and Lights
 * @{
 */
// LED 1 (RA0)
#define LED1_TRIS TRISAbits.TRISA0  /**< Status LED 1 Direction Register */
#define LED1_LAT  LATAbits.LATA0    /**< Status LED 1 Latch Register */

// Button T1 (RE8)
#define BTN_T1_TRIS TRISEbits.TRISE8 /**< Button T1 Direction Register */
#define BTN_T1_PORT PORTEbits.RE8    /**< Button T1 Port Reading Register */

// Button T2 (RE9)
#define BTN_T2_TRIS TRISEbits.TRISE9 /**< Button T2 Direction Register */
#define BTN_T2_PORT PORTEbits.RE9    /**< Button T2 Port Reading Register */

// Low-intensity lights (RG1)
#define LIL_TRIS TRISGbits.TRISG1   /**< Low Intensity Lights Direction Register */
#define LIL_LAT  LATGbits.LATG1     /**< Low Intensity Lights Latch Register */

// Left-side light  (RB8)
#define LSL_TRIS TRISBbits.TRISB8   /**< Left Side Light Direction Register */
#define LSL_LAT  LATBbits.LATB8     /**< Left Side Light Latch Register */

// Right-side light (RF1)
#define RSL_TRIS TRISFbits.TRISF1   /**< Right Side Light Direction Register */
#define RSL_LAT  LATFbits.LATF1     /**< Right Side Light Latch Register */
/** @} */

/** --------------------------------------------------
 * @name System Error Messages
 * @{
 */
#define MSG_ERR_INPUT "$ERR,1*\n"   /**< Standard serial error response for invalid commands. */
/** @} */

#endif // CONFIG_H