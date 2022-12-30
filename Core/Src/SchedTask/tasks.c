/*
 * tasks.c
 *
 *  Created on: Dec 30, 2022
 *      Author: ntpt
 */

#include "main.h"
#include "SchedTask/tasks.h"
#include "Hardware/buzzer.h"
#include "Logical/global_object.h"
#include "Logical/fsm.h"

void update_button_task(void *param)
{
    Hardware_Button_PollAndUpdate(&prev_button);
    Hardware_Button_PollAndUpdate(&next_button);
    Hardware_Button_PollAndUpdate(&state_button);
    Hardware_Button_PollAndUpdate(&pedestrian_button);
}

void run_fsm_task(void *param)
{
    Fsm_t *fsm = (Fsm_t*) param;
    Logical_Fsm_SetNextState(fsm);
    Logical_Fsm_DoInState(fsm);
}

void toggle_buzzer_task(void *param)
{
    Hardware_Buzzer_Toggle(&buzzer);
}

void one_second_timer_task(void *param)
{
    if (time_left > 0 && need_time_check)
        time_left--;
}
