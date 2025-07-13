#include "scheduler.h"


Task tasks[4];

void Scheduler_Run(void)
{
    uint32_t system_time = HAL_GetTick();

    for (int i = 0; i < TASK_COUNT; ++i) {
        if (system_time - tasks[i].last_run >= tasks[i].interval_ms) {
            tasks[i].last_run = system_time;
            tasks[i].task_func();
        }
    }
}






