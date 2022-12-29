/*
 * scheduler.h
 *
 *  Created on: Nov 13, 2022
 *      Author: ntpt
 */

#ifndef INC_LOGICAL_SCHEDULER_H_
#define INC_LOGICAL_SCHEDULER_H_

#include <Logical/error.h>
#include <Logical/priority_queue.h>
#include <Logical/scheduler_task.h>
#include "main.h"

/*
 * NOTE:
 * Here we define the required code and static variable for the cooperative scheduler.
 * A regular timer is required by this scheduler to run. A watchdog timer can be
 * configured to be used within this scheduler. It will reset the system if a task is
 * taking too long, using the IWDG timer
 *
 * Since we will only have one scheduler in the whole system, we don't need to package
 * all the required data into a separate struct. All we provide is a API for using the
 * statically allocated scheduler and its associated data structure (also statically
 * allocated).
 *
 * This cooperative scheduler support adding task (one-shot or auto-reload) with
 * duration and priority. Deleting task is also support. Both adding and deleting can
 * be performed when the scheduler is running.
 *
 * Policy for the scheduler, as mentioned is cooperative. Meaning that each task is run
 * until completion until finished. Only after that another can start running. Cooperative
 * policy opens the task to starvation.
 *
 * Each task is represented by a struct 'SchedTask_t', which will we stored in a
 * array representing a binary heap. The task have a runAtTick property to store the value
 * of tick that the task is scheduled to be run at.  Each task will take a function
 * pointer (with void * argument) and it is that function that will be called when
 * the task is run. Note that there is no counter within each task to count-down till
 * execute (but a system absolute timestamp (tick)).
 *
 * It is assumed the order established for the binary heap is provided by a comparison
 * function.
 *
 * The API for the scheduler have these function:
 * - Custom_Scheduler_Init()
 *   Initialize the system (regular timer, watchdog timer if configured).
 * - Custom_Scheduler_Update()
 *   This function is called in the regular timer ISR, it increments the global time for
 *   whole system.
 * - Custom_Scheduler_Add()
 *   This function add a 'SchedTask_t' struct within the priority queue (binary heap) and
 *   at the correct position. It takes various argument enough for establising a new task.
 *   Which is: task function pointer, argument pointer, priority value, period, delay (from
 *   scheduler start or from adding time) and task ID.
 * - Custom_Scheduler_Delete()
 *   Remove a task from the priority queue, taking the task ID identifying the task to
 *   remove.
 * - Custom_Scheduler_Dispatch()
 *   This function is intended to be called within the super loop, it will determined the
 *   next task to be run and run it. After finishing all task that are need to be run, it
 *   put the uC into sleep state until the system is wake up again by regular timer
 *   interrupt.
 */

// config for hardware watchdog timer
// the specified IWDG will be initialized in Custom_Scheduler_Init and be used exclusively
// make sure that nothing else uses it
#undef CUSTOM_SCHEDULER_USE_WATCHDOG

// config for regular timer (timer 3)
// the timer will be initialized and start in interrupt mode in Custom_Scheduler_Init and
// be used exclusively, make sure that nothing else uses it
// user have to run Custom_Scheduler_Update within the timer callback
// assume that the initialization code set the timer duration equal to the tick duration
#define CUSTOM_SCHEDULER_TICK_DURATION_MS 10

// config for the priority queue (binary heap)
// default to setting the size equal to a complete binary tree of depth n
// though different size value is okay, it is recommended to set size to 2^n - 1
#define CUSTOM_SCHEDULER_BIHEAP_HEIGHT 5
#define CUSTOM_SCHEDULER_BIHEAP_SIZE ((1u << CUSTOM_SCHEDULER_BIHEAP_HEIGHT) - 1)

// defining the ordering between task (comparison function)
//
// this function have a default implementation (weak), user can overwrite that
// and implement a custom function within user code
//
// NOTE: must return true (1) if task1 < task2 to create a max heap in which
// task2 ranks higher than task1
uint8_t Custom_SchedTask_Compare_Smaller(void* task1, void *task2);

// time conversion macro
// turn from time duration in ms to number of tick
#define CUSTOM_SCHEDULER_MS_TO_TICK(ms) (ms / CUSTOM_SCHEDULER_TICK_DURATION_MS)



// prototype for scheduler API
void Logical_Scheduler_Init(void);
void Logical_Scheduler_Update(void);
void Logical_Scheduler_Add(SchedTask_Func_t pTask, void *pArg,
        uint8_t priority, uint32_t period, uint32_t delay, uint8_t ID);
void Logical_Scheduler_Delete(uint8_t ID);
void Logical_Scheduler_Dispatch();

#endif /* INC_LOGICAL_SCHEDULER_H_ */
