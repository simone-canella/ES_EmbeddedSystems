/**
 * @file BOSCH_BMX055.h
 * @brief Header file for the Bosch BMX055 IMU SPI driver.
 */

#ifndef BOSCH_BMX055_H
#define BOSCH_BMX055_H

    #include "config.h"

    /** @brief Target identifier for the Accelerometer. */
    #define ACC 1
    /** @brief Target identifier for the Gyroscope. */
    #define GYR 2
    /** @brief Target identifier for the Magnetometer. */
    #define MAG 3

    /** @brief Chip Select pin mapping for the Accelerometer. */
    #define ACC_CS LATBbits.LATB3 // RB3 accelerometer
    /** @brief Chip Select pin mapping for the Gyroscope. */
    #define GYR_CS LATBbits.LATB4 // RB4 gyroscope
    /** @brief Chip Select pin mapping for the Magnetometer. */
    #define MAG_CS LATDbits.LATD6 // RD6 magnetometer 

    /**
     * @struct IMUdata
     * @brief Container for a 3-axis spatial measurement.
     */
    typedef struct {
        int x; /**< Formatted measurement for the X-axis. */
        int y; /**< Formatted measurement for the Y-axis. */
        int z; /**< Formatted measurement for the Z-axis. */
    } IMUdata;

    /**
     * @brief Initializes the BMX055 sensor and configures the SPI bus parameters.
     */
    void BMX055_setup();

    /**
     * @brief Executes the wake-up sequence for the BMX055 magnetometer.
     */
    void BMX055_mag_wakeup();

    /**
     * @brief Reads a full frame of multi-axis data from the specified IMU sensor.
     * @param sensor The target sensor peripheral to read (ACC, GYR, or MAG).
     * @return IMUdata A structure containing the extracted x, y, and z axis measurements.
     */
    IMUdata imuRead(int sensor);

#endif // BOSCH_BMX055_H