/*
 * priority_queue.h
 *
 *  Created on: Nov 10, 2022
 *      Author: ntpt
 */

#ifndef INC_LOGICAL_PRIORITY_QUEUE_H_
#define INC_LOGICAL_PRIORITY_QUEUE_H_

#include "main.h"

// it is implied that if this function perform the operation
// arg1 < arg2 (sematically speaking), then the resulting queue
// will be a max-queue
typedef uint8_t (*Compare_function_t)(void*, void*);

/*
 * NOTE:
 * This module define function that work on a statically allocated priority
 * queue (binary heap). Whether it is max-heap or min-heap is not important,
 * since we will provide the comparison function between element.
 *
 * It is assumed that the array containing the heap already exists, and
 * we can access the top element of the heap at array[0]
 *
 * The operations defined for the heap are:
 * - heap_create: make heap from random array
 * - heap_insert: insert a element into the heap
 * - heap_pop: remove the top element of the heap
 * - heap_delete: remove the element at specified index
 * - heap_push_down: try to push down the element at the top
 *
 * This module is written with reuse in mind, so it is necessarily general
 * and unoptimized. Each function takes a slew of argument:
 * - pointer to array containing the heap
 * - array max size (in element count)
 * - element size (in bytes)
 * - current element count within array
 * - compare function
 * - pointer to the element to add (insert into head)
 */

void Logical_PQueue_Create(void *arr, size_t asize, size_t esize, size_t elemc,
        Compare_function_t cmp);
void Logical_PQueue_Insert(void *arr, size_t asize, size_t esize, size_t elemc, void *elem,
        Compare_function_t cmp);
void Logical_PQueue_Pop(void *arr, size_t asize, size_t esize, size_t elemc,
        Compare_function_t cmp);
void Logical_PQueue_Delete(void *arr, size_t asize, size_t esize, size_t elemc, size_t index,
        Compare_function_t cmp);
void Logical_PQueue_PushDown(void *arr, size_t esize, size_t elemc,
        Compare_function_t cmp);

#endif /* INC_LOGICAL_PRIORITY_QUEUE_H_ */
