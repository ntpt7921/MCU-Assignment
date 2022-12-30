/*
 * fsm_state_list.h
 *
 *  Created on: Nov 1, 2022
 *      Author: ntpt
 */

#ifndef INC_USER_FSM_STATE_LIST_H_
#define INC_USER_FSM_STATE_LIST_H_

typedef enum
{
    // normal state
    AM_GR, // 0
    AM_YR, // 1
    AM_RG, // 2
    AM_RY, // 3

    // pedestrian scramble state
    AM_SCR_12, // occurs between 1 and 2
    AM_SCR_30, // occurs between 0 and 3

    // tuning state
    T_G, // tune for green
    T_Y, // tune for yellow
    T_SCR, // tune for scramble
} FsmState_t;

#endif /* INC_USER_FSM_STATE_LIST_H_ */
