/*
 * button.h
 *
 *  Created on: Oct 26, 2022
 *      Author: ntpt
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

/**
 * NOTE:
 * Each button can have 3 separate state: released, pressed and hold
 *
 * hold state will be entered after being in pressed state for a certain duration
 *
 * each state is treated as mutually exclusive, meaning that state can not overlap
 * so button in hold state is not considered to be in pressed state (even if common
 * sense tell you otherwise)
 */
typedef enum
{
	BUTTON_IS_RELEASED, BUTTON_IS_PRESSED, BUTTON_IS_HOLD
} Button_state_t;

/**
 * NOTE:
 * button read can have two reading value: GPIO_PIN_RESET and GPIO_PIN_SET
 */
typedef GPIO_PinState Button_read_value_t;

/**
 * NOTE:
 * since this code is written to accommodate both type of pressed state
 * (low when pressed, high when pressed), the associated pressed state
 * will be store for each button
 *
 * button will be software debounced by waiting for some N stable
 * continuous read value of the button pin
 *
 * a flag variable is set when button change state from released to pressed (flag = 1)
 * or from pressed to hold (flag = 2) or from holding for a period (flag = 2)
 */
typedef struct
{
	// variable for configuration of the button
	const uint8_t FILTER_DELAY_COUNT_MAX;
	const uint16_t HOLD_STATE_DELAY_COUNT_MAX;
	const uint16_t HOLD_STATE_ACTIVATE_COUNT_MAX;
	const Button_read_value_t READING_WHEN_PRESSED;
	const uint16_t PIN;
	GPIO_TypeDef *const PORT; // const pointer to GPIO_TypeDef

	Button_state_t current_state;

	// variables for internal state of button
	Button_read_value_t last_reading;
	uint8_t filter_delay_count;
	uint16_t hold_state_delay_count;
	uint16_t hold_state_activate_count;
	uint8_t activate_flag;
} Button_t;

void Hardware_Button_PollAndUpdate(volatile Button_t *bt);
uint8_t Hardware_Button_IsPressed(volatile Button_t *bt);
uint8_t Hardware_Button_IsHeld(volatile Button_t *bt);
uint8_t Hardware_Button_InvokedByPress(volatile Button_t *bt);
uint8_t Hardware_Button_InvokedByHold(volatile Button_t *bt);
void Hardware_Button_ClearFlag(volatile Button_t *bt);

#endif /* INC_BUTTON_H_ */
