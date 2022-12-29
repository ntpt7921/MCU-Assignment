/*
 * scheduler_task.h
 *
 *  Created on: Nov 10, 2022
 *      Author: ntpt
 */

#ifndef INC_LOGICAL_SCHEDULER_TASK_H_
#define INC_LOGICAL_SCHEDULER_TASK_H_

#include "main.h"

/*
 * NOTE:
 * Each task should have a associated function, which we defined ask SchedTask_Func_t
 * The function should have the form "void function_name(void *param)"
 */
typedef void (*SchedTask_Func_t)(void*);

/*
 * NOTE:
 * The scheduler_task_t is a structure containing all the required infomation
 * to represent a task in our scheduler implemetation
 *
 * The priority of a test is determined as according to a rule: compare by priority,
 * then compare by runAtTick next
 */
typedef struct
{
    SchedTask_Func_t pTask;  // function pointer
    void *pTaskArg;          // argument for task
    uint8_t priority;        // priority for task
    uint32_t runAtTick;      // scheduled to be run at tick
    uint32_t periodTick;     // period for auto-reload task, 0 if not auto-reload
    uint8_t taskID;          // used to identify task
} SchedTask_t;


#endif /* INC_LOGICAL_SCHEDULER_TASK_H_ */
