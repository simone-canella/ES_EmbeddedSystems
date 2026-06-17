#ifndef SPI_H
#define SPI_H

#include <math.h>
#include <stdbool.h>
#include <xc.h>

// Function Prototypes
void spi1_mikrobus2_setup(void);
unsigned int spi1_write(unsigned int data);

#endif