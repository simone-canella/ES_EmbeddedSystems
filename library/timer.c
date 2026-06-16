/**
 * @file timer.c
 * @brief Implementation of hardware timer drivers for dsPIC33.
 */

#include "timer.h"

#define F_CY 72000000UL // 72MHz Internal Instruction Clock
#define MAX_TICKS 65535 // 16-bit Timer Limit (2^16 - 1)

/**
 * @brief Configures a timer to expire after a specific duration.
 * @param timer The timer to use (TIMER1 or TIMER2).
 * @param ms Duration in milliseconds (Max 200ms).
 *
 * @details this function serves as configuration for the function
 * "tmr_wait_period"
 */
void tmr_setup_period(int timer, int ms) {
	long ticks_per_ms = F_CY / 1000;
	long total_ticks = ticks_per_ms * ms;

	int prescaler_bits = 0;
	int prescaler_divisor = 1;

	// Determine the smallest prescaler_bits that keeps ticks < 65535
	if ((total_ticks / 1) < MAX_TICKS) {
		prescaler_bits = 0b00; // 1:1 Prescaler
		prescaler_divisor = 1;
	} else if ((total_ticks / 8) < MAX_TICKS) {
		prescaler_bits = 0b01; // 1:8 Prescaler
		prescaler_divisor = 8;
	} else if ((total_ticks / 64) < MAX_TICKS) {
		prescaler_bits = 0b10; // 1:64 Prescaler
		prescaler_divisor = 64;
	} else {
		prescaler_bits = 0b11; // 1:256 Prescaler
		prescaler_divisor = 256;
	}

	int target_period = total_ticks / prescaler_divisor; // target period value

	switch (timer) {
		case TIMER1:
			T1CONbits.TON = 0; // stop the timer for configuring parameters

			T1CONbits.TCS = 0; // use the internal clock as source (F_cy)

			T1CONbits.TCKPS = prescaler_bits; // set the prescaler value

			TMR1 = 0; // clear the current count

			PR1 = target_period; // set target period

			IFS0bits.T1IF = 0; // reset the interrupt's flag

			T1CONbits.TON = 1; // start the timer count

			break;

		case TIMER2:
			T2CONbits.TON = 0; // stop the timer for configuring parameters

			T2CONbits.TCS = 0; // use the internal clock as source (F_cy)

			T2CONbits.TCKPS = prescaler_bits; // set the prescaler value

			TMR2 = 0; // clear the current count

			PR2 = target_period; // set target period

			IFS0bits.T2IF = 0; // reset the interrupt's flag

			T2CONbits.TON = 1; // start the timer count
			break;

		case TIMER3:
			T3CONbits.TON = 0; // stop the timer for configuring parameters

			T3CONbits.TCS = 0; // use the internal clock as source (F_cy)

			T3CONbits.TCKPS = prescaler_bits; // set the prescaler value

			TMR3 = 0; // clear the current count

			PR3 = target_period; // set target period

			IFS0bits.T3IF = 0; // reset the interrupt's flag

			T3CONbits.TON = 1; // start the timer count
			break;

		case TIMER4:
			T4CONbits.TON = 0; // stop the timer for configuring parameters

			T4CONbits.TCS = 0; // use the internal clock as source (F_cy)

			T4CONbits.TCKPS = prescaler_bits; // set the prescaler value

			TMR4 = 0; // clear the current count

			PR4 = target_period; // set target period

			IFS1bits.T4IF = 0; // reset the interrupt's flag

			T4CONbits.TON = 1; // start the timer count
			break;

		case TIMER5:
			T5CONbits.TON = 0; // stop the timer for configuring parameters

			T5CONbits.TCS = 0; // use the internal clock as source (F_cy)

			T5CONbits.TCKPS = prescaler_bits; // set the prescaler value

			TMR5 = 0; // clear the current count

			PR5 = target_period; // set target period

			IFS1bits.T5IF = 0; // reset the interrupt's flag

			T5CONbits.TON = 1; // start the timer count
			break;

		default:
			break;
	}
}

/**
 * @brief Halts execution until a periodic timer expires, checking for scheduling violations.
 * @details Captures the hardware interrupt flag state instantly upon entering the function. 
 * If the flag is already set, it indicates the main application loop took too long 
 * to execute its workload, violating real-time task constraints.
 * @param timer The active hardware timer register to monitor (TIMER1 or TIMER2).
 * @return int Returns 1 if a deadline miss occurred, or 0 if execution met the schedule timeline.
 */
int tmr_wait_period(int timer) {
	int missed_deadline = 0;

	switch (timer) {
		case TIMER1:
			missed_deadline = IFS0bits.T1IF;

			while (!IFS0bits.T1IF) {
				// if the interrupt's flag is not "1" busy waiting
			}

			IFS0bits.T1IF = 0; // reset the flag
			break;

		case TIMER2:
			missed_deadline = IFS0bits.T2IF;

			while (!IFS0bits.T2IF) {
				// if the interrupt's flag is not "1" busy waiting
			}

			IFS0bits.T2IF = 0; // reset the flag
			break;

		case TIMER3:
			missed_deadline = IFS0bits.T3IF;

			while (!IFS0bits.T3IF) {
				// if the interrupt's flag is not "1" busy waiting
			}

			IFS0bits.T3IF = 0; // reset the flag
			break;

		case TIMER4:
			missed_deadline = IFS1bits.T4IF;

			while (!IFS1bits.T4IF) {
				// if the interrupt's flag is not "1" busy waiting
			}

			IFS1bits.T4IF = 0; // reset the flag
			break;
		
		case TIMER5:
			missed_deadline = IFS1bits.T5IF;

			while (!IFS1bits.T5IF) {
				// if the interrupt's flag is not "1" busy waiting
			}

			IFS1bits.T5IF = 0; // reset the flag
			break;

		default:
			break;
	}

	return missed_deadline;
}

/**
 * @brief Generates an on-demand millisecond delay using loop-chunking mechanics.
 * @details Slices the total duration input into safe, predictable 200 ms segments. 
 * This bypasses the 233 ms register capacity limit of the 16-bit architecture, 
 * processing full cycles sequentially using a loop, followed by an execution block 
 * for any remaining fractional duration.
 * 
 * @param timer The hardware timer instance allocated to compute the delay.
 * @param ms The total requested waiting period duration in milliseconds.
 */
void tmr_wait_ms(int timer, int ms) {
    int full_chunks = ms / 200; // Determine number of isolated 200ms segments
    int remainder = ms % 200;   // Capture remaining fractional milliseconds

    // Process all large, full 200ms blocks sequentially
    for (int i = 0; i < full_chunks; i++) {
        tmr_setup_period(timer, 200);
        
        if (timer == TIMER1) {
            while (IFS0bits.T1IF == 0); // Polling trap
            IFS0bits.T1IF = 0;          // Acknowledge match event
            T1CONbits.TON = 0;          // Turn off module to prepare for next step
        } else {
            while (IFS0bits.T2IF == 0);
            IFS0bits.T2IF = 0;
            T2CONbits.TON = 0;
        }
    }

    // Process any remaining fractional milliseconds to complete accuracy profile
    if (remainder > 0) {
        tmr_setup_period(timer, remainder);
        
        if (timer == TIMER1) {
            while (IFS0bits.T1IF == 0); // Polling trap
            IFS0bits.T1IF = 0;          // Clear flag
            T1CONbits.TON = 0;          // Deactivate module
        } else {
            while (IFS0bits.T2IF == 0);
            IFS0bits.T2IF = 0;
            T2CONbits.TON = 0;
        }
    }
}