/*
 * queue.h
 *
 *  Created on: Nov 13, 2022
 *      Author: ntpt
 */

#ifndef INC_LOGICAL_CIRCULAR_BUFFER_H_
#define INC_LOGICAL_CIRCULAR_BUFFER_H_

#include "main.h"

/*
 * NOTE:
 * This module define function that work on a circular buffer, stored
 * within a statically allocated array.
 *
 * It is assumed that the array containing the buffer already exist, and
 * two other counter (head - pointing to the buffer head, count - counting
 * number of element currently in the buffer) must be provided by the user.
 * The next value to be read is at array[head], new value will be inserted
 * at array[(head + count) % size]
 *
 * Operations defined for the circular buffer:
 * - buffer_insert: write a new element into the buffer if there is space
 * - buffer_delete: increment head if there is element left to delete
 *
 * After operation on the buffer, the value of head and count will be
 * updated accordingly. When the operation can not be performed, the buffer
 * remains unchanged.
 *
 * This module is written with reuse in mind, so it is necessarily general
 * and unoptimized. Operation takes various paramenter:
 * - pointer to array containing the buffer
 * - buffer max size (in element count)
 * - element size (in bytes)
 * - pointer to head and count
 * - pointer to the element to add (insert into buffer)
 */

void Logical_CirBuff_Insert(void *arr, size_t asize, size_t esize,
        size_t *head, size_t *count, void *elem);
void Logical_CirBuff_Delete(size_t asize, size_t *head, size_t *count);

#endif /* INC_LOGICAL_CIRCULAR_BUFFER_H_ */
