/**
 * @file scheduler.h
 * @brief Cooperative non-preemptive task scheduler header for the robot buggy.
 * @author Simone Canella, Samuele Isaia, Riccardo Parotelli
 * @version 1.0.0
 *
 * @details This module declares a lightweight cyclic scheduler operating on a fixed 
 * time base. It maintains a tracking matrix of registered system tasks and updates 
 * execution flags at every interval tick. Tasks execute completely to completion 
 * without nested thread interruption.
 *
 * NOTE: 
 * - The system tick interval is centralized here. 
 * - All registered tasks must execute quickly to prevent timing overruns on subsequent tasks.
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "timer.h"
#include <stddef.h> 

/**
 * @brief Base scheduling time slice definition in milliseconds.
 * @details Defines the baseline physical clock period matching the hardware timer setup.
 */
#define tick_ms 2       // tick duration in ms 


// --- Core Public Scheduler Interfaces ---

/**
 * @brief Main task evaluation and execution loop handler.
 * @return void
 *
 * @details This routine must be called inside the main infinite processing loop. 
 * It iterates through the matrix of registered tasks, decrements their remaining 
 * delay counts, and calls the function pointers of active tasks whose intervals 
 * have expired.
 */
void scheduler();

/**
 * @brief Registers a new periodic task into the background execution matrix.
 * @param initial_offset The starting delay in ticks before the first task execution occurs.
 * @param period_ticks The repetition interval of the task measured in system clock ticks.
 * @param f Function pointer targeting the specific task callback function.
 * @param arg Generic void pointer containing optional data parameters passed to the task.
 * @return void
 *
 * @details Adds a task structure into the internal registration array. The offset parameter 
 * is highly critical here: it is used to interleave task execution frames, ensuring 
 * that the processor worst-case scenario load is evenly balanced across separate ticks.
 */
void register_task(int initial_offset, int period_ticks, void (*f)(void *), void *arg);

/**
 * @brief Enables a registered task to resume its periodic execution loops.
 * @param task_ID The numeric identifier index of the target task.
 * @return void
 *
 * @details Modifies the control flag of the specified task index, authorizing the 
 * execution loop engine to evaluate its periodic counters again.
 */
void enable_task(int task_ID);

/**
 * @brief Disables a registered task to temporarily halt its execution loop.
 * @param task_ID The numeric identifier index of the target task.
 * @return void
 *
 * @details Suspends a task from being triggered by clearing its active run flag. 
 * The task remains in memory with its scheduling context preserved, but it is 
 * skipped during the scheduler execution sweeps.
 */
void disable_task(int task_ID);

#endif // SCHEDULER_H