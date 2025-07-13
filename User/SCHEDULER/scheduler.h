#ifndef __SCHEDULER_H
#define __SCHEDULER_H 

#include "main.h"
#include "stdlib.h"	

typedef struct {
    uint32_t interval_ms;
    uint32_t last_run;
    void (*task_func)(void);
} Task;

#define TASK_COUNT (sizeof(tasks)/sizeof(tasks[0]))
	



#endif





