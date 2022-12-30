/*
 * button.c
 *
 *  Created on: Oct 26, 2022
 *      Author: ntpt
 */

#include "Hardware/button.h"

void Hardware_Button_PollAndUpdate(volatile Button_t *bt)
{
    Button_read_value_t current_reading = HAL_GPIO_ReadPin(bt->PORT, bt->PIN);

    // filter input and update to pressed or released state
    if (bt->last_reading == current_reading)
    {
        bt->filter_delay_count++;

        if (bt->filter_delay_count >= bt->FILTER_DELAY_COUNT_MAX)
        {
            if (current_reading == bt->READING_WHEN_PRESSED)
            {
                // only update state to pressed if previous state was released
                // and set the flag
                if (bt->current_state == BUTTON_IS_RELEASED)
                {
                    bt->activate_flag = 1;
                    bt->current_state = BUTTON_IS_PRESSED;
                }
            }
            else
            {
                bt->current_state = BUTTON_IS_RELEASED;
            }
            bt->filter_delay_count = 0;
        }
    }
    else
    {
        bt->filter_delay_count = 0;
    }

    // within pressed state, after a delay has passed, change to hold state
    // also activate the flag
    if (bt->current_state == BUTTON_IS_PRESSED)
    {
        bt->hold_state_delay_count++;

        if (bt->hold_state_delay_count >= bt->HOLD_STATE_DELAY_COUNT_MAX)
        {
            bt->current_state = BUTTON_IS_HOLD;
            bt->activate_flag = 2;
            bt->hold_state_delay_count = 0;
        }
    }
    else
    {
        bt->hold_state_delay_count = 0;
    }

    // within hold state, after a delay has passed, reactivate the flag
    if (bt->current_state == BUTTON_IS_HOLD)
    {
        bt->hold_state_activate_count++;

        if (bt->hold_state_activate_count >= bt->HOLD_STATE_ACTIVATE_COUNT_MAX)
        {
            bt->activate_flag = 2;
            bt->hold_state_activate_count = 0;
        }
    }
    else
    {
        bt->hold_state_activate_count = 0;
    }

    bt->last_reading = current_reading;
}

uint8_t Hardware_Button_IsPressed(volatile Button_t *bt)
{
    return (bt->current_state == BUTTON_IS_PRESSED);
}

uint8_t Hardware_Button_IsHeld(volatile Button_t *bt)
{
    return (bt->current_state == BUTTON_IS_HOLD);
}

uint8_t Hardware_Button_InvokedByPress(volatile Button_t *bt)
{
    return (bt->activate_flag == 1);
}

uint8_t Hardware_Button_InvokedByHold(volatile Button_t *bt)
{
    return (bt->activate_flag == 2);

}
void Hardware_Button_ClearFlag(volatile Button_t *bt)
{
    bt->activate_flag = 0;
}
