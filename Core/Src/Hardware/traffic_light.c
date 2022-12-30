/*
 * traffic_light.c
 *
 *  Created on: Dec 29, 2022
 *      Author: ntpt
 */

#include "Hardware/traffic_light.h"
#include "stm32f1xx_hal_gpio.h"

void Hardware_TrafficLight_SetColor(TrafficLight_t *tf, TrafficLightColor_t color)
{
    switch (color)
    {
        case COLOR_NONE:
            // 00
            HAL_GPIO_WritePin(tf->port[0], tf->pin[0], GPIO_PIN_RESET);
            HAL_GPIO_WritePin(tf->port[1], tf->pin[1], GPIO_PIN_RESET);
            break;
        case COLOR_RED:
            // 10
            HAL_GPIO_WritePin(tf->port[0], tf->pin[0], GPIO_PIN_RESET);
            HAL_GPIO_WritePin(tf->port[1], tf->pin[1], GPIO_PIN_SET);
            break;
        case COLOR_YELLOW:
            // 11
            HAL_GPIO_WritePin(tf->port[0], tf->pin[0], GPIO_PIN_SET);
            HAL_GPIO_WritePin(tf->port[1], tf->pin[1], GPIO_PIN_SET);
            break;
        case COLOR_GREEN:
            // 01
            HAL_GPIO_WritePin(tf->port[0], tf->pin[0], GPIO_PIN_SET);
            HAL_GPIO_WritePin(tf->port[1], tf->pin[1], GPIO_PIN_RESET);
            break;
        default:
            // do nothing
            break;
    }
}
void Hardware_TrafficLight_TurnOff(TrafficLight_t *tf)
{
    Hardware_TrafficLight_SetColor(tf, COLOR_NONE);
}

