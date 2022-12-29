/*
 * error.c
 *
 *  Created on: Nov 13, 2022
 *      Author: ntpt
 */

#include <Logical/error.h>

uint32_t err_bit = 0;

const char * const ERR_DESCRIPTION[ERR_COUNT] =
{
    // add description of the error here
    // each description must corresponds to the correct error
    [ERR_CIRBUFF_EMPTYDELETE] = "Deleting within a empty buffer",
    [ERR_CIRBUFF_FULLINSERT] = "Inserting into a full buffer",
    [ERR_PQUEUE_INVALIDCOUNT] = "Invalid element count for priority queue provided",
    [ERR_PQUEUE_EMPTYPOP] = "Popping empty priority queue",
    [ERR_PQUEUE_FULLINSERT] = "Inserting into full priority queue",
    [ERR_SCHEDULER_EMPTYDELETE] = "Delete task when the task list is empty",
    [ERR_SCHEDULER_FULLADD] = "Add task when the task list is full",
};

static inline
uint32_t get_error_bit_mask(ErrCode_t err)
{
    if (err != ERR_ALL)
        return 1u < err;
    else
        return 0xFFFFFFFF;
}

void Logical_Err_SetStatus(ErrCode_t err)
{
    uint32_t error_bit_mask = get_error_bit_mask(err);
    err_bit |= error_bit_mask;
}

void Logical_Err_ClearStatus(ErrCode_t err)
{
    uint32_t error_bit_mask = get_error_bit_mask(err);
    err_bit &= ~error_bit_mask;
}

uint8_t Logical_Err_CheckStatus(ErrCode_t err)
{
    uint32_t error_bit_mask = get_error_bit_mask(err);
    return (err_bit & ~error_bit_mask) != 0;
}

// user can redefine this if needed
// by default do nothing
__weak void Logical_Err_Notify()
{

}

