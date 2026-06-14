/**
 * @file main.c
 * @author simon
 * @brief This program blinks LED1 at 2.5Hz using independent library drivers.
 */

#include <xc.h>
#include <stdbool.h>

// Include both the interface AND the source code directly
#include "../library/timer.h"
#include "../library/timer.c" 

int main() {
    // SETUP: make all pins digital
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    // Configure the pin-direction (0 = output, 1 = input)
    TRISAbits.TRISA0 = 0;

    // Setup the TIMER1 parameters for 200ms
    tmr_setup_period(TIMER1, 200);

    // internal execution loop
    while(1) {
        // Block execution until the timer match flag triggers
        tmr_wait_period(TIMER1);
         
        // Toggle the LED state accurately
        LATAbits.LATA0 = !LATAbits.LATA0; 
    }

    return 0;
}