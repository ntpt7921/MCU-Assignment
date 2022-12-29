/*
 * fsm.h
 *
 *  Created on: Nov 1, 2022
 *      Author: ntpt
 */

#include <Logical/fsm.h>

FsmState_t Logical_Fsm_GetNextState(Fsm_t *fsm)
{
    return fsm->get_next_state(fsm->current_state);
}

void Logical_Fsm_SetNextState(Fsm_t *fsm)
{
    FsmState_t next_state = Logical_Fsm_GetNextState(fsm);
    if (fsm->set_next_state == NULL)
    {
        fsm->set_next_state(next_state);
    }
    fsm->current_state = next_state;
}

void Logical_Fsm_DoInState(Fsm_t *fsm)
{
    if (fsm->do_in_state == NULL)
    {
        fsm->do_in_state();
    }
}
