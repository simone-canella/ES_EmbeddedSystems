#include "BOSCH_BMX055.h"

volatile unsigned int DATA_LSB, DATA_MSB;

void BMX055_setup(){
    // Bosch BMX055 SPI protocol require idle LOW 

    SPI1STATbits.SPIEN = 0;     // disable SPI to change settings

    SPI1CON1bits.CKE = 1; // Output data changes on transition from active to idle
    SPI1CON1bits.CKP = 0; // Idle state for clock is a low level

    SPI1STATbits.SPIEN = 1;     // re-enable SPI

    tmr_wait_ms(TIMER1, 100);
}

void BMX055_mag_wakeup(){
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
int format12bit(){
    DATA_LSB = spi1_write(0x00);
    DATA_MSB = spi1_write(0x00);
    DATA_LSB = DATA_LSB & 0b11110000;
    DATA_MSB = DATA_MSB << 8;
    return (int) (DATA_MSB | DATA_LSB)/16;
}

int format13bit(){
    DATA_LSB = spi1_write(0x00);
    DATA_MSB = spi1_write(0x00);
    DATA_LSB = DATA_LSB & 0b11111000;
    DATA_MSB = DATA_MSB << 8;
    return (int) (DATA_MSB | DATA_LSB)/8;
}

int format15bit(){
    DATA_LSB = spi1_write(0x00);
    DATA_MSB = spi1_write(0x00);
    DATA_LSB = DATA_LSB & 0b11111110;
    DATA_MSB = DATA_MSB << 8;
    return (int) (DATA_MSB | DATA_LSB)/2;
}

int format16bit(){
    DATA_LSB = spi1_write(0x00);
    DATA_MSB = spi1_write(0x00);
    DATA_MSB = DATA_MSB << 8;
    return (int) (DATA_MSB | DATA_LSB);
}

sensordata imuRead(int sensor){
    sensordata data;

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