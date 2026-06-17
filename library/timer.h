#ifndef TIMER_H
#define TIMER_H

#include <math.h>
#include <stdbool.h>
#include <xc.h>

#define TIMER1 1
#define TIMER2 2
#define TIMER3 3
#define TIMER4 4
#define TIMER5 5

void tmr_setup_period(int timer, int ms);
int tmr_wait_period(int timer);
void tmr_wait_ms(int timer, int ms);

#endif