/**
 * @file main.c
 * @author simon
 * @date 2026-06-14
 * @brief Main function
 */

 #include <xc.h>
 #include "../library/timer.h"
 #include "../library/timer.c"

int main(){
    // PIN SETUP: all the pins are digital
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;

    // Configure pin direction (0 = output, 1 = input)
    TRISAbits.TRISA0 = 0;

    // Execution-loop
    while(1) {
        // turn-on LED1 for 20ms
        LATAbits.LATA0 = 1;
        tmr_wait_ms(TIMER1, 20);

        // turn-off LED1 for 200ms
        LATAbits.LATA0 = 0;
        tmr_wait_ms(TIMER1, 200);
    }

    return 0;
}
