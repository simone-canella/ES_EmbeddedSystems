// Include Guard -> tells the compiler to include (and compile) this header only once, even if multiple scripts calls it

#ifndef TIMER_H_
#define TIMER_H_

#include <xc.h>

#define TIMER1 1
#define TIMER2 2
#define TIMER3 3
#define TIMER4 4
#define TIMER5 5

#define TMREXP   1  // timer expired flag
#define NOTMR   -2  // timer is not defined

// Function prototypes
int tmr_setup_period(int tmr, float ms);
int tmr_wait_period(int tmr);
int tmr_wait_ms(int tmr, float ms);

#endif // TIMER_H_