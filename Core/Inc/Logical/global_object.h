/*
 * global_object.h
 *
 *  Created on: Nov 1, 2022
 *      Author: ntpt
 */

#ifndef INC_LOGICAL_GLOBAL_OBJECT_H_
#define INC_LOGICAL_GLOBAL_OBJECT_H_

#include "Hardware/button.h"
#include "Hardware/buzzer.h"
#include "Hardware/traffic_light.h"
#include "Logical/fsm.h"

extern Button_t prev_button;
extern Button_t next_button;
extern Button_t state_button;
extern Button_t pedestrian_button;

extern TrafficLight_t normal_tfl1;
extern TrafficLight_t normal_tfl2;
extern TrafficLight_t pedestrian_tfl;

extern Buzzer_t buzzer;

extern uint32_t green_duration;
extern uint32_t yellow_duration;
extern uint32_t red_duration;
extern uint32_t scr_duration;
extern uint32_t time_left;
extern uint8_t  need_time_check;
extern uint8_t  scramble_requested;

extern Fsm_t fsm;

#endif /* INC_LOGICAL_GLOBAL_OBJECT_H_ */
