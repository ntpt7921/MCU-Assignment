/*
 * error.h
 *
 *  Created on: Nov 13, 2022
 *      Author: ntpt
 */

#ifndef INC_LOGICAL_ERROR_H_
#define INC_LOGICAL_ERROR_H_

#include "main.h"


/*
 * NOTE:
 * When error occurs, a associated bit within err_bit will be set to 1.
 * Since the bit length of err_bit is 32, there are a maximum of 32 unique
 * error that can be reported.
 *
 * If a error bit is already set, setting it again have to effect.
 *
 * User are provided with the following operation:
 * - set status: change the associated bit in err_bit
 * - clean status: reset error bit associated with the provided error
 * - notify: try to notify the error through custom, user defined way
 * - check status: check if the provided error code is set
 */

typedef enum
{
    ERR_CIRBUFF_EMPTYDELETE = 0,
    ERR_CIRBUFF_FULLINSERT,

    ERR_PQUEUE_INVALIDCOUNT,
    ERR_PQUEUE_EMPTYPOP,
    ERR_PQUEUE_FULLINSERT,

    ERR_SCHEDULER_FULLADD,
    ERR_SCHEDULER_EMPTYDELETE,

    ERR_COUNT = 32, // the maximum value that this should have is 32
    ERR_ALL, // used to refer to all error bit
} ErrCode_t;

extern const char * const ERR_DESCRIPTION[ERR_COUNT];
extern uint32_t err_bit;

void Logical_Err_SetStatus(ErrCode_t err);
void Logical_Err_ClearStatus(ErrCode_t err);
uint8_t Logical_Err_CheckStatus(ErrCode_t err);
void Logical_Err_Notify();

#endif /* INC_LOGICAL_ERROR_H_ */
