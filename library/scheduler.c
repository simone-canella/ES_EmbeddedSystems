#include "scheduler.h"

void scheduler(heartbeat schedInfo[], int nTasks) {
    for (int i = 0; i < nTasks; i++) {
        schedInfo[i].n++;
        if (schedInfo[i].enable == 1 && schedInfo[i].n >= schedInfo[i].N) {
            schedInfo[i].f(schedInfo[i].params); // Run the task
            schedInfo[i].n = 0;                  // Reset counter
        }
    }
}