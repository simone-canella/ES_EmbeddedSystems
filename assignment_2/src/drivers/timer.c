/**
 * @file timer.c
 * @brief Hardware timer configuration and delay management routines.
 * * This file implements the setup and control functions for hardware timers 
 * TIMER1 through TIMER5. It calculates prescaler and period register values 
 * dynamically based on a 72 MHz system instruction frequency (Fcy) to provide 
 * accurate timing and blocking delays.
 */

#include "timer.h"

// Fcy = 72MHz. Defined by the bootloader 

/**
 * @brief Configures and starts a specific timer for a given period.
 * * Dynamically selects the optimal prescaler (1:8, 1:64, or 1:256) based on 
 * the target milliseconds to avoid register overflow. It resets the timer, 
 * clears its interrupt flag, configures the period register (PRx), and starts it.
 * * @param tmr The hardware timer to configure (e.g., TIMER1, TIMER2, ..., TIMER5).
 * @param ms The desired timer period in milliseconds. Max supported period is ~232 ms.
 * @return int Returns 0 on successful configuration, or NOTMR if an invalid timer is passed.
 */
int tmr_setup_period(int tmr, float ms){
    float cpms;
    int prescaler;

    if(ms < 7){
        cpms = 9000.00; // Fcy * prescaler * 0.001 (ms) -> fino a quanto devo contare per avere 1ms
        prescaler = 0b01;
    } else if (ms < 58){
        cpms = 1125.00;
        prescaler = 0b10;
    } else if (ms < 233){
        cpms = 281.25;
        prescaler = 0b11;
    }
    
    switch(tmr){
        case TIMER1:
            T1CON = 0x0000; // reinitialize the timer (by default TGATE = 0 and TCS = 0)
            PR1 = (int)(cpms * ms); // set the target
            TMR1 = 0;               // reset timer counter
            T1CONbits.TCKPS = prescaler; // set the prescaler
            
            IFS0bits.T1IF = 0; // clear the flag
            T1CONbits.TON = 1; // start the timer

            break;

        case TIMER2:
            T2CON = 0x0000;
            PR2 = (int)(cpms * ms);
            TMR2 = 0;    
            T2CONbits.TCKPS = prescaler;
            
            IFS0bits.T2IF = 0;
            T2CONbits.TON = 1;

            break;

        case TIMER3:
            T3CON = 0x0000;
            PR3 = (int)(cpms * ms);
            TMR3 = 0;    
            T3CONbits.TCKPS = prescaler;
            
            IFS0bits.T3IF = 0;
            T3CONbits.TON = 1;

            break;

        case TIMER4:
            T4CON = 0x0000;
            PR4 = (int)(cpms * ms);
            TMR4 = 0;    
            T4CONbits.TCKPS = prescaler;
            
            IFS1bits.T4IF = 0;
            T4CONbits.TON = 1;

            break;
        
        case TIMER5:
            T5CON = 0x0000;
            PR5 = (int)(cpms * ms);
            TMR5 = 0;    
            T5CONbits.TCKPS = prescaler;
            
            IFS1bits.T5IF = 0;
            T5CONbits.TON = 1;

            break;

        default:
            return NOTMR;
    }

    return 0;
};

/**
 * @brief Performs a blocking wait until the specified timer period expires.
 * * Polls the hardware interrupt flag for the given timer. If the flag is already 
 * set when this function is called, it detects a missed deadline. Otherwise, 
 * it holds execution in a while-loop until the hardware timer trips the flag.
 * * @param tmr The hardware timer to monitor (e.g., TIMER1, TIMER2, ..., TIMER5).
 * @return int Returns 0 upon successful wait completion, TMREXP if the deadline 
 * was missed before polling began, or NOTMR for an invalid timer.
 */
int tmr_wait_period(int tmr){
    switch(tmr){
        case TIMER1:
            if (IFS0bits.T1IF == 1){ // Deadline missed: the period expired before we started waiting
                IFS0bits.T1IF = 0;   // Clear it before returning
                return TMREXP;       // Attention: i DO NO know how many time it has expired
            }

            while (IFS0bits.T1IF == 0){
                // Wait for the timer flag to trigger
            }

            IFS0bits.T1IF = 0;  // Reset the flag
            break;

        case TIMER2:
            if (IFS0bits.T2IF == 1){ 
                IFS0bits.T2IF = 0;
                return TMREXP;       
            }

            while (IFS0bits.T2IF == 0){}

            IFS0bits.T2IF = 0;
            break;

        case TIMER3:
            if (IFS0bits.T3IF == 1){ 
                IFS0bits.T3IF = 0;
                return TMREXP;       
            }

            while (IFS0bits.T3IF == 0){}

            IFS0bits.T3IF = 0;
            break;

        case TIMER4:
            if (IFS1bits.T4IF == 1){ 
                IFS1bits.T4IF = 0;
                return TMREXP;       
            }

            while (IFS1bits.T4IF == 0){}

            IFS1bits.T4IF = 0;
            break;

        case TIMER5:
            if (IFS1bits.T5IF == 1){ 
                IFS1bits.T5IF = 0;
                return TMREXP;       
            }

            while (IFS1bits.T5IF == 0){}

            IFS1bits.T5IF = 0;
            break;

        default:
            return NOTMR;
    }

    return 0; // no error
};

/**
 * @brief Executes a synchronous, one-shot blocking delay.
 * * A convenience wrapper that automatically sets up the timer, waits for the 
 * requested period to expire, and cleanly shuts down the timer afterward to 
 * prevent continuous background counting.
 * * @param tmr The hardware timer to use for the delay (e.g., TIMER1, TIMER2, ..., TIMER5).
 * @param ms The duration to block execution, in milliseconds.
 * @return int Returns 0 upon completion, or NOTMR if an invalid timer is passed.
 */
int tmr_wait_ms(int tmr, float ms){ //use the timer to wait for a given number of milliseconds given as input. 

    tmr_setup_period(tmr, ms);
    tmr_wait_period(tmr);

    switch(tmr){
        case TIMER1:
            TMR1 = 0;           // reset timer counter
            T1CONbits.TON = 0;  // stops the timer
            break;

        case TIMER2:
            TMR2 = 0;    
            T2CONbits.TON = 0;
            break;

        case TIMER3:
            TMR3 = 0;    
            T3CONbits.TON = 0;
            break;
    
        case TIMER4:
            TMR4 = 0;    
            T4CONbits.TON = 0;
            break;

        case TIMER5:
            TMR5 = 0;    
            T5CONbits.TON = 0;
            break;

        default:
            return NOTMR;
    }

    return 0; // no error
};