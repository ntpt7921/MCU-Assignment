/*
 * fsm.h
 *
 *  Created on: Nov 1, 2022
 *      Author: ntpt
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "main.h"
#include <Logical/fsm_state_list.h>

/**
 * NOTE:
 * The finite state machine (FSM) module is written for general use by user
 *
 * FSM will be represented by a struct, containing the current state and 3 function pointer:
 * - to get the next state
 * - to perform action when changing to the next state
 * - to perform action within a specified state
 *
 * The user will be provided with 3 corresponding function to invoke the associated action.
 *
 * FSM_state_t is declared in fsm_state_list.h
 */

typedef FsmState_t (*FsmFunction_GetNextState_t)(FsmState_t current_state);
typedef void (*FsmFunction_SetNextState_t)(FsmState_t next_state);
typedef void (*FsmFunction_DoInState_t)(FsmState_t current_state);

typedef struct
{
	FsmState_t current_state;
	FsmFunction_GetNextState_t get_next_state;
	FsmFunction_SetNextState_t set_next_state;
	FsmFunction_DoInState_t do_in_state;
} Fsm_t;

/**
 * NOTE:
 * These 3 function represent the changing of state of the FSM
 */

FsmState_t Logical_Fsm_GetNextState(Fsm_t *fsm);
void Logical_Fsm_SetNextState(Fsm_t *fsm);
void Logical_Fsm_DoInState(Fsm_t *fsm);

#endif /* INC_FSM_H_ */
