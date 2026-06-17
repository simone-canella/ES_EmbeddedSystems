/** * @file scheduler.c
 * @brief Cooperative cyclic task scheduler implementation for the robot buggy platform.
 * @author Simone Canella, Samuele Isaia, Riccardo Parotelli
 * @version 1.0.0
 */

#include "config.h"
#include "scheduler.h"

#define MAX_TASKS 16

/**
 * @brief Internal structure to represent a scheduled task.
 */
typedef struct {
    int n;              /**< Current tick counter for the task. */
    int N;              /**< Tick period for the task execution. */
    void (*f)(void *);  /**< Function pointer to the task payload. */
    void *arg;          /**< Argument to pass to the task function. */
    int enable;         /**< Enable flag: 1 if active, 0 if disabled. */
} tasky;

/** @brief Array of registered tasks. */
static tasky task[MAX_TASKS];
/** @brief The total number of tasks currently registered. */
static int task_count = 0; 

/**
 * @brief Core scheduler routine that evaluates and executes ready tasks.
 */
void scheduler(void) {
    for (int i = 0; i < task_count; i++) {
        if (task[i].enable == 1) {
            task[i].n++;
            if (task[i].n >= task[i].N) {
                task[i].f(task[i].arg); 
                task[i].n = 0;    
            }
        }
    }
}

/**
 * @brief Registers a new task in the scheduler queue.
 * @param initial_offset The initial offset in ticks before first execution.
 * @param period_ticks The recurrence period in ticks.
 * @param f Function pointer to the task routine.
 * @param arg Pointer to pass to the task routine as an argument.
 */
void register_task(int initial_offset, int period_ticks, void (*f)(void *), void *arg) {
    if (task_count < MAX_TASKS) {
        task[task_count].f = f;
        task[task_count].arg = arg;
        task[task_count].N = period_ticks;
        task[task_count].n = initial_offset;
        task[task_count].enable = 1;
        task_count++;
    }
}

/**
 * @brief Enables a registered task.
 * @param task_ID The index of the task to enable.
 */
void enable_task(int task_ID){
    task[task_ID].enable = 1;
};

/**
 * @brief Disables a registered task.
 * @param task_ID The index of the task to disable.
 */
void disable_task(int task_ID){
    task[task_ID].enable = 0;
};