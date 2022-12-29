/*
 * software_timer.h
 *
 *  Created on: Oct 24, 2022
 *      Author: ntpt
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include "main.h"

#define TIMER_TICK_DURATION_MS 10

typedef enum
{
	TIMER_FLAG_RESET = RESET, TIMER_FLAG_SET = SET,
} SoftTimer_FlagState_t;


typedef struct
{
	uint32_t timer_counter;
	SoftTimer_FlagState_t timer_flag;
} SoftTimer_t;

void Logical_SoftTimer_SetDurationTick(volatile SoftTimer_t *tm, uint32_t ms);
void Logical_SoftTimer_TickUpdate(volatile SoftTimer_t *tm);
uint8_t Logical_SoftTimer_IsSet(volatile SoftTimer_t *tm);

#endif /* INC_SOFTWARE_TIMER_H_ */
