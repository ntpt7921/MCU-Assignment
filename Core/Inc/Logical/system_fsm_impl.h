/*
 * system_fsm_impl.h
 *
 *  Created on: Dec 30, 2022
 *      Author: ntpt
 */

#ifndef INC_LOGICAL_SYSTEM_FSM_IMPL_H_
#define INC_LOGICAL_SYSTEM_FSM_IMPL_H_

#include "Logical/fsm.h"

FsmState_t system_get_next_state(FsmState_t current_state);
void system_set_next_state(FsmState_t next_state);
void system_do_in_state(FsmState_t current_state);

#endif /* INC_LOGICAL_SYSTEM_FSM_IMPL_H_ */
