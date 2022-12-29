/*
 * buzzer.c
 *
 *  Created on: Dec 29, 2022
 *      Author: ntpt
 */

#include "Hardware/buzzer.h"

void Hardware_Buzzer_Init(void)
{
    // use hardware timer 3 to create 50% PWM signal for buzzer
    MX_TIM3_Init();
}

void Hardware_Buzzer_On(Buzzer_t *b)
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    b->state = BUZZER_ON;
}

void Hardware_Buzzer_Off(Buzzer_t *b)
{
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    b->state = BUZZER_OFF;
}

void Hardware_Buzzer_Toggle(Buzzer_t *b)
{
    if (b->state == BUZZER_ON)
    {
        Hardware_Buzzer_Off(b);
    }
    else if (b->state == BUZZER_OFF)
    {
        Hardware_Buzzer_On(b);
    }
    else
    {
        // yeah, every thing burn down here
    }
}
