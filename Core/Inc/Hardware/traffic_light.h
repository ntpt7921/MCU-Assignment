/*
 * traffic_light.h
 *
 *  Created on: Dec 29, 2022
 *      Author: ntpt
 */

#ifndef INC_HARDWARE_TRAFFIC_LIGHT_H_
#define INC_HARDWARE_TRAFFIC_LIGHT_H_

#include "main.h"
#include "stm32f103xb.h"

typedef enum
{
    COLOR_RED = 0,
    COLOR_YELLOW,
    COLOR_GREEN,
    COLOR_NONE,
} TrafficLightColor_t;

typedef struct
{
    TrafficLightColor_t current_color;
    GPIO_TypeDef *port[2];
    uint16_t pin[2];

} TrafficLight_t;

void Hardware_TrafficLight_SetColor(TrafficLight_t *tf, TrafficLightColor_t color);
void Hardware_TrafficLight_TurnOff(TrafficLight_t *tf);

#endif /* INC_HARDWARE_TRAFFIC_LIGHT_H_ */
