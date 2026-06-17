/**
 * @file timer.h
 * @brief Header file for hardware timer configuration and delay management routines.
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <xc.h>

/** @brief Identifier for hardware TIMER1. */
#define TIMER1 1
/** @brief Identifier for hardware TIMER2. */
#define TIMER2 2
/** @brief Identifier for hardware TIMER3. */
#define TIMER3 3
/** @brief Identifier for hardware TIMER4. */
#define TIMER4 4
/** @brief Identifier for hardware TIMER5. */
#define TIMER5 5

/** @brief Error code flag indicating the timer expired before waiting began. */
#define TMREXP   1  // timer expired flag
/** @brief Error code flag indicating the specified timer is not valid. */
#define NOTMR   -2  // timer is not defined

/**
 * @brief Configures and starts a specific timer for a given period.
 * @param tmr The hardware timer to configure (e.g., TIMER1, TIMER2, ..., TIMER5).
 * @param ms The desired timer period in milliseconds.
 * @return int Returns 0 on successful configuration, or NOTMR if an invalid timer is passed.
 */
int tmr_setup_period(int tmr, float ms);

/**
 * @brief Performs a blocking wait until the specified timer period expires.
 * @param tmr The hardware timer to monitor (e.g., TIMER1, TIMER2, ..., TIMER5).
 * @return int Returns 0 upon successful wait completion, TMREXP if deadline missed, or NOTMR on error.
 */
int tmr_wait_period(int tmr);

/**
 * @brief Executes a synchronous, one-shot blocking delay.
 * @param tmr The hardware timer to use for the delay (e.g., TIMER1, TIMER2, ..., TIMER5).
 * @param ms The duration to block execution, in milliseconds.
 * @return int Returns 0 upon completion, or NOTMR if an invalid timer is passed.
 */
int tmr_wait_ms(int tmr, float ms);

#endif // TIMER_H_