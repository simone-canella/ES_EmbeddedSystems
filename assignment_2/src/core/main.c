/**
 * @file main.c
 * @brief Application entry point and main scheduler loop for the buggy.
 * @author Simone Canella, Samuele Isaia, Riccardo Parotelli
 * @version 1.0.0
 */

#include "config.h"
#include "robot_state.h"
#include "fsm.h"
#include "tasks.h" 
#include "scheduler.h"

/** @brief Counter tracking the number of missed scheduler deadlines. */
int deadlineMissed;

/**
 * @brief Main execution function.
 * @return Application exit code.
 */
int main(void) {
    static RobotState_t rover_state;
    
    hw_init(&rover_state);
    fsm_init(&rover_state);
    tasks_init(&rover_state);
    
    while(1) {
        scheduler();
        deadlineMissed += tmr_wait_period(TIMER1);
    }
    
    return 0;
}