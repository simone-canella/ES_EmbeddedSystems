#ifndef SPI_H
#define SPI_H

    #include "timer.h"
    #include <xc.h>

    // Function prototypes
    void spi1_mikrobus1_setup();
    unsigned int spi1_write(unsigned int data);

#endif // SPI_H