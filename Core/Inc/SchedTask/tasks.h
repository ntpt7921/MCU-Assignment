/*
 * tasks.h
 *
 *  Created on: Dec 30, 2022
 *      Author: ntpt
 */

#ifndef INC_SCHEDTASK_TASKS_H_
#define INC_SCHEDTASK_TASKS_H_

#define UPDATE_BUTTON_TASK_ID 0
#define RUN_FSM_TASK_ID 1
#define TOGGLE_BUZZER_TASK_ID 2
#define ONE_SECOND_TIMER_TASK_ID 3

void update_button_task(void *param);
void run_fsm_task(void *param);
void toggle_buzzer_task(void *param);
void one_second_timer_task(void *param);

#endif /* INC_SCHEDTASK_TASKS_H_ */
