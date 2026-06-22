#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <xc.h>

typedef struct {
    int n;          // Current heartbeat count (elapsed heartbeats since last execution)
    int N;          // Target heartbeat count (target heartbeat threshold required to trigger task)
    int enable;     // 1 = Active, 0 = Paused
    void (*f)(void*); // Function pointer to the task
    void* params;   // Pointer to task data (structs, etc.)
} heartbeat;

void scheduler(heartbeat schedInfo[], int nTasks);

#endif