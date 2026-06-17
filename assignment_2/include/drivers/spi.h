/**
 * @file spi.h
 * @brief Header file for the SPI interface driver and hardware configuration.
 */

#ifndef SPI_H
#define SPI_H

    #include "config.h"

    /**
     * @brief Configures the SPI1 peripheral pins, control registers, and default chip selects.
     */
    void spi1_mikrobus1_setup();

    /**
     * @brief Transmits and receives a single byte over the SPI bus.
     * @param data The 8-bit data payload to transmit.
     * @return unsigned int The 8-bit data payload received from the slave device.
     */
    unsigned int spi1_write(unsigned int data);

#endif // SPI_H