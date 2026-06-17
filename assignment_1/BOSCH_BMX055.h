#ifndef BOSCH_BMX055
#define BOSCH_BMX055

    #include "timer.h"
    #include "spi.h"
    #include <xc.h>

    #define ACC 1
    #define GYR 2
    #define MAG 3

    #define ACC_CS LATBbits.LATB3 // RB3 accelerometer
    #define GYR_CS LATBbits.LATB4 // RB4 gyroscope
    #define MAG_CS LATDbits.LATD6 // RD6 magnetometer 

    typedef struct {
        int x;
        int y;
        int z;
    } sensordata;

    // Function prototypes
    void BMX055_setup();
    void BMX055_mag_wakeup();

    // reading data
    sensordata imuRead();


#endif // BOSCH_BMX055