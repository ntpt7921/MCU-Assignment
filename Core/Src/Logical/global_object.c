/*
 * global_object.c
 *
 *  Created on: Nov 1, 2022
 *      Author: ntpt
 */

#include "Hardware/buzzer.h"
#include "Hardware/traffic_light.h"
#include "Logical/fsm_state_list.h"
#include "Logical/global_object.h"
#include "Logical/system_fsm_impl.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal_gpio.h"

Button_t prev_button =
{
    .FILTER_DELAY_COUNT_MAX = 5,
    .HOLD_STATE_DELAY_COUNT_MAX = 50,
    .HOLD_STATE_ACTIVATE_COUNT_MAX = 200,
    .READING_WHEN_PRESSED = GPIO_PIN_RESET,
    .PIN = GPIO_PIN_1,
    .PORT = GPIOA,
};

Button_t next_button =
{
    .FILTER_DELAY_COUNT_MAX = 5,
    .HOLD_STATE_DELAY_COUNT_MAX = 50,
    .HOLD_STATE_ACTIVATE_COUNT_MAX = 200,
    .READING_WHEN_PRESSED = GPIO_PIN_RESET,
    .PIN = GPIO_PIN_4,
    .PORT = GPIOA,
};

Button_t state_button =
{
    .FILTER_DELAY_COUNT_MAX = 5,
    .HOLD_STATE_DELAY_COUNT_MAX = 50,
    .HOLD_STATE_ACTIVATE_COUNT_MAX = 200,
    .READING_WHEN_PRESSED = GPIO_PIN_RESET,
    .PIN = GPIO_PIN_0,
    .PORT = GPIOB,
};

Button_t pedestrian_button =
{
    .FILTER_DELAY_COUNT_MAX = 5,
    .HOLD_STATE_DELAY_COUNT_MAX = 50,
    .HOLD_STATE_ACTIVATE_COUNT_MAX = 200,
    .READING_WHEN_PRESSED = GPIO_PIN_RESET,
    .PIN = GPIO_PIN_0,
    .PORT = GPIOA,
};

TrafficLight_t normal_tfl1 =
{
    .current_color = COLOR_NONE,
    .port = { GPIOA, GPIOB },
    .pin = { GPIO_PIN_10, GPIO_PIN_3 },
};

TrafficLight_t normal_tfl2 =
{
    .current_color = COLOR_NONE,
    .port = { GPIOB, GPIOB },
    .pin = { GPIO_PIN_5, GPIO_PIN_4 },
};

TrafficLight_t pedestrian_tfl =
{
    .current_color = COLOR_NONE,
    .port = { GPIOB, GPIOA },
    .pin = { GPIO_PIN_10, GPIO_PIN_8 },
};

Buzzer_t buzzer =
{
    .state = BUZZER_OFF,
};

uint32_t green_duration  = 3;
uint32_t yellow_duration = 2;
uint32_t red_duration    = 5;
uint32_t scr_duration    = 5;
uint32_t time_left       = 0;
uint8_t  need_time_check = 1;
uint8_t  scramble_requested = 0;

Fsm_t fsm =
{
    .current_state  = AM_GR,
    .do_in_state    = system_do_in_state,
    .get_next_state = system_get_next_state,
    .set_next_state = system_set_next_state,
};
