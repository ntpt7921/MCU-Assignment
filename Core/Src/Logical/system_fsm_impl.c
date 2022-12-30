/*
 * system_fsm_impl.c
 *
 *  Created on: Dec 30, 2022
 *      Author: ntpt
 */

#include "Hardware/button.h"
#include "Hardware/traffic_light.h"
#include "Logical/fsm.h"
#include "Logical/fsm_state_list.h"
#include "Logical/global_object.h"
#include "Logical/scheduler.h"
#include "Logical/system_fsm_impl.h"
#include "SchedTask/tasks.h"
#include "main.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

// used as buffer for UART
uint8_t buffer[100] = { 0 };

static inline
uint8_t check_state_time_elapsed(void)
{
    if (need_time_check)
    {
        return (time_left == 0);
    }
    else
    {
        return 0; // no need for time check, always return true
    }

    // not suppose to reach this
    return 0;
}

static inline
FsmState_t next_state_from_current(FsmState_t current_state)
{
    switch (current_state)
    {
        case AM_GR:
            return AM_YR;
        case AM_YR:
            if (scramble_requested)
                return AM_SCR_12;
            else
                return AM_RG;
        case AM_RG:
            return AM_RY;
        case AM_RY:
            if (scramble_requested)
                return AM_SCR_30;
            else
                return AM_GR;

        case AM_SCR_12:
            return AM_RG;
        case AM_SCR_30:
            return AM_GR;

        case T_G:
            return T_Y;
        case T_Y:
            return T_SCR;
        case T_SCR:
            return T_G;

        default:
            return AM_GR;
    }
}

static inline
FsmState_t prev_state_from_current(FsmState_t current_state)
{
    switch (current_state)
    {
        case AM_GR:
            if (scramble_requested)
                return AM_SCR_30;
            else
                return AM_RY;
        case AM_YR:
            return AM_GR;
        case AM_RG:
            if (scramble_requested)
                return AM_SCR_12;
            else
                return AM_YR;
        case AM_RY:
            return AM_RG;

        case AM_SCR_12:
            return AM_YR;
        case AM_SCR_30:
            return AM_RY;

        // no prev state for tuning, these states only go forward

        default:
            return AM_GR;
    }
}

FsmState_t system_get_next_state(FsmState_t current_state)
{
    FsmState_t normal_next_state = next_state_from_current(current_state);
    FsmState_t normal_prev_state = prev_state_from_current(current_state);

    switch (current_state)
    {
        case AM_GR:
        case AM_YR:
        case AM_RG:
        case AM_RY:
        case AM_SCR_12:
        case AM_SCR_30:
            if (Hardware_Button_InvokedByHold(&state_button))
            {
                Hardware_Button_ClearFlag(&state_button);
                return T_G;
            }
            if (check_state_time_elapsed())
                return normal_next_state;
            else if (Hardware_Button_InvokedByPress(&next_button))
            {
                Hardware_Button_ClearFlag(&next_button);
                return normal_next_state;
            }
            else if (Hardware_Button_InvokedByPress(&prev_button))
            {
                Hardware_Button_ClearFlag(&prev_button);
                return normal_prev_state;
            }
            break;

        case T_G:
        case T_Y:
        case T_SCR:
            if (Hardware_Button_InvokedByPress(&state_button))
            {
                Hardware_Button_ClearFlag(&state_button);
                return normal_next_state;
            }
            if (Hardware_Button_InvokedByHold(&state_button))
            {
                Hardware_Button_ClearFlag(&state_button);
                return AM_GR;
            }
            break;

        default:
            return current_state;
    }

    return current_state;
}

static inline
uint32_t min(uint32_t a, uint32_t b)
{
    return (a < b) ? a : b;
}

static inline
void set_timeout_for_state(FsmState_t state)
{
    switch (state)
    {
        case AM_GR:
            time_left = min(green_duration, red_duration);
            break;
        case AM_YR:
            time_left = min(yellow_duration, red_duration);
            break;
        case AM_RG:
            time_left = min(red_duration, green_duration);
            break;
        case AM_RY:
            time_left = min(green_duration, yellow_duration);
            break;
        case AM_SCR_12:
            time_left = scr_duration;
            break;
        case AM_SCR_30:
            time_left = scr_duration;
            break;

        // tune mode don't check for timeout, so don't need to set it

        default:
            break;
    }
}

static inline
void set_traffic_light_for_state(FsmState_t state)
{
    switch (state)
    {
        case AM_GR:
            Hardware_TrafficLight_TurnOff(&pedestrian_tfl);
            Hardware_TrafficLight_SetColor(&normal_tfl1, COLOR_GREEN);
            Hardware_TrafficLight_SetColor(&normal_tfl2, COLOR_RED);
            break;
        case AM_YR:
            Hardware_TrafficLight_TurnOff(&pedestrian_tfl);
            Hardware_TrafficLight_SetColor(&normal_tfl1, COLOR_YELLOW);
            Hardware_TrafficLight_SetColor(&normal_tfl2, COLOR_RED);
            break;
        case AM_RG:
            Hardware_TrafficLight_TurnOff(&pedestrian_tfl);
            Hardware_TrafficLight_SetColor(&normal_tfl1, COLOR_RED);
            Hardware_TrafficLight_SetColor(&normal_tfl2, COLOR_GREEN);
            break;
        case AM_RY:
            Hardware_TrafficLight_TurnOff(&pedestrian_tfl);
            Hardware_TrafficLight_SetColor(&normal_tfl1, COLOR_RED);
            Hardware_TrafficLight_SetColor(&normal_tfl2, COLOR_YELLOW);
            break;

        case AM_SCR_12:
            Hardware_TrafficLight_SetColor(&pedestrian_tfl, COLOR_GREEN);
            Hardware_TrafficLight_SetColor(&normal_tfl1, COLOR_RED);
            Hardware_TrafficLight_SetColor(&normal_tfl2, COLOR_RED);
            break;
        case AM_SCR_30:
            Hardware_TrafficLight_SetColor(&pedestrian_tfl, COLOR_GREEN);
            Hardware_TrafficLight_SetColor(&normal_tfl1, COLOR_RED);
            Hardware_TrafficLight_SetColor(&normal_tfl2, COLOR_RED);
            break;

        case T_G:
            Hardware_TrafficLight_TurnOff(&pedestrian_tfl);
            Hardware_TrafficLight_SetColor(&normal_tfl1, COLOR_GREEN);
            Hardware_TrafficLight_SetColor(&normal_tfl2, COLOR_GREEN);
            break;
        case T_Y:
            Hardware_TrafficLight_TurnOff(&pedestrian_tfl);
            Hardware_TrafficLight_SetColor(&normal_tfl1, COLOR_YELLOW);
            Hardware_TrafficLight_SetColor(&normal_tfl2, COLOR_YELLOW);
            break;
        case T_SCR:
            Hardware_TrafficLight_TurnOff(&pedestrian_tfl);
            Hardware_TrafficLight_SetColor(&normal_tfl1, COLOR_RED);
            Hardware_TrafficLight_SetColor(&normal_tfl2, COLOR_RED);
            break;

        default:
            break;
    }
}

void print_msg_on_enterring_state(FsmState_t state)
{
    switch (state)
    {
        case AM_GR:
            sprintf((char*) buffer, "Green-red (0)\r\n");
            break;
        case AM_YR:
            sprintf((char*) buffer, "Yellow-red (1)\r\n");
            break;
        case AM_RG:
            sprintf((char*) buffer, "Red-green (2)\r\n");
            break;
        case AM_RY:
            sprintf((char*) buffer, "Red-yellow (3)\r\n");
            break;
        case AM_SCR_12:
            sprintf((char*) buffer, "Scramble (1-2)\r\n");
            break;
        case AM_SCR_30:
            sprintf((char*) buffer, "Scramble (3-0)\r\n");
            break;
        case T_G:
            sprintf((char*) buffer, "Tune green\r\n");
            break;
        case T_Y:
            sprintf((char*) buffer, "Tune yellow\r\n");
            break;
        case T_SCR:
            sprintf((char*) buffer, "Tune scramble\r\n");
            break;

        default:
            break;
    }
    // send notice, block maximum 10ms
    HAL_UART_Transmit(&huart2, buffer, strlen((const char*) buffer), 10);
}

void system_set_next_state(FsmState_t next_state)
{
    switch (next_state)
    {
        case AM_GR:
        case AM_YR:
        case AM_RG:
        case AM_RY:
        case AM_SCR_12:
        case AM_SCR_30:
        case T_G:
        case T_Y:
        case T_SCR:
            set_timeout_for_state(next_state);
            set_traffic_light_for_state(next_state);
            print_msg_on_enterring_state(next_state);
            break;

        default:
            break;
    }

    if (next_state == AM_SCR_12 || next_state == AM_SCR_30)
    {
        scramble_requested = 0; // done servicing
    }

    if (next_state == T_G || next_state == T_Y || next_state == T_SCR)
    {
        Logical_Scheduler_Add(toggle_buzzer_task, NULL, 0, MS_TO_SCHEDTICK(500), 0, TOGGLE_BUZZER_TASK_ID);
    }
    else
    {
        Logical_Scheduler_Delete(TOGGLE_BUZZER_TASK_ID);
    }
}

static inline
void change_state_duration(FsmState_t state, uint32_t amount)
{
    switch (state)
    {
        case T_G:
            green_duration += amount;
            sprintf((char*) buffer, "%"PRIu32 "\r\n", green_duration);
            break;
        case T_Y:
            yellow_duration += amount;
            sprintf((char*) buffer, "%"PRIu32 "\r\n", yellow_duration);
            break;
        case T_SCR:
            scr_duration += amount;
            sprintf((char*) buffer, "%"PRIu32 "\r\n", scr_duration);
            break;

        default:
            break;
    }

    red_duration = yellow_duration + green_duration;

    if (amount != 0)
    {
        HAL_UART_Transmit(&huart2, buffer, strlen((const char*) buffer), 10);
    }
}

void system_do_in_state(FsmState_t current_state)
{
    switch (current_state)
    {
        case AM_GR:
        case AM_YR:
        case AM_RG:
        case AM_RY:
        case AM_SCR_12:
        case AM_SCR_30:
            if (Hardware_Button_InvokedByPress(&pedestrian_button)
                    || Hardware_Button_InvokedByHold(&pedestrian_button))
            {
                Hardware_Button_ClearFlag(&pedestrian_button);
                if (!scramble_requested)
                {
                    scramble_requested = 1;
                    sprintf((char*) buffer, "Scramble requested\r\n");
                    HAL_UART_Transmit(&huart2, buffer, strlen((const char*) buffer), 10);
                }
            }
            if (Hardware_Button_InvokedByPress(&state_button))
            {
                Hardware_Button_ClearFlag(&state_button);
                need_time_check = !need_time_check; // invert 0->1, 1->0
                if (need_time_check)
                    sprintf((char*) buffer, "Automatic mode enabled\r\n");
                else
                    sprintf((char*) buffer, "Manual mode enabled\r\n");
                HAL_UART_Transmit(&huart2, buffer, strlen((const char*) buffer), 10);
            }
            break;

        case T_G:
        case T_Y:
        case T_SCR:
            if (Hardware_Button_InvokedByPress(&next_button)
                    || Hardware_Button_InvokedByHold(&next_button))
            {
                Hardware_Button_ClearFlag(&next_button);
                change_state_duration(current_state, 1);
            }
            else if (Hardware_Button_InvokedByPress(&prev_button)
                    || Hardware_Button_InvokedByHold(&prev_button))
            {
                Hardware_Button_ClearFlag(&prev_button);
                change_state_duration(current_state, -1);
            }
            break;


        default:
            break;
    }
}
