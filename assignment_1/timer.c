#include "timer.h"

// Fcy = 72MHz. Defined by the bootloader 

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