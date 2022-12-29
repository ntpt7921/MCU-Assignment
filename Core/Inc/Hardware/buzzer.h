/*
 * buzzer.h
 *
 *  Created on: Dec 29, 2022
 *      Author: ntpt
 */

#ifndef INC_HARDWARE_BUZZER_H_
#define INC_HARDWARE_BUZZER_H_

#include "main.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal_tim.h"
#include "tim.h"

typedef enum
{
    BUZZER_ON,
    BUZZER_OFF,
} BuzzerState_t;

typedef struct
{
    BuzzerState_t state;
} Buzzer_t;

void Hardware_Buzzer_Init(void);
void Hardware_Buzzer_On(Buzzer_t *b);
void Hardware_Buzzer_Off(Buzzer_t *b);
void Hardware_Buzzer_Toggle(Buzzer_t *b);

#endif /* INC_HARDWARE_BUZZER_H_ */
