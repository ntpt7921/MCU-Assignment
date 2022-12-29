/*
 * priority_queue.c
 *
 *  Created on: Nov 10, 2022
 *      Author: ntpt
 */

#include <Logical/error.h>
#include <Logical/priority_queue.h>
#include <string.h>

// create a temp mem (used for swapping)
// the size is arbitrarily set (must be bigger than the size of
// elements we are swapping.
// also try to allocate aligned by 4 bytes
#define MAX_TEMP_MEM_SIZE_BYTES  (32u << 2u) // allocate 32 bytes
static uint8_t temp_mem[MAX_TEMP_MEM_SIZE_BYTES];

static inline void swap(void *e1, void *e2, size_t size)
{
    memcpy(temp_mem, e1, size);
    memcpy(e1, e2, size);
    memcpy(e2, temp_mem, size);
}

static inline size_t get_parent_index(size_t child_index)
{
    return (child_index - 1) / 2;
}

static inline size_t get_child_one_index(size_t parent_index)
{
    return parent_index * 2 + 1;
}

static inline size_t get_child_two_index(size_t parent_index)
{
    return parent_index * 2 + 2;
}

static inline void *get_element_address(void *array, size_t elem_size, size_t index)
{
    return (void*) ((uint8_t*) array + elem_size * index);
}

// will try to sift up the designated element (index elem_index)
static void sift_up(void *array,
                    size_t elem_size, size_t elem_index,
                    Compare_function_t cmp)
{
    size_t parent_index = get_parent_index(elem_index);
    while (elem_index != 0)
    {
        void *parent_address = get_element_address(array, elem_size, parent_index);
        void *current_address = get_element_address(array, elem_size, elem_index);

        if (cmp(parent_address, current_address))
        {
            swap(parent_address, current_address, elem_size);
            elem_index = parent_index;
            parent_index = get_parent_index(elem_index);
        }
        else
        {
            break;
        }
    }
}

// will try to sift down the designated element (at index elem_index)
static void sift_down(void *array, size_t arr_max_size,
                      size_t elem_size, size_t elem_index,
                      Compare_function_t cmp)
{
    while (elem_index < arr_max_size)
    {
        // if child_one (left) index is larger the array element count
        // we have reached the end of the array, so top
        size_t child_one_index = get_child_one_index(elem_index);
        if (child_one_index >= arr_max_size)
        {
            break;
        }
        void *child_one_address = get_element_address(array, elem_size, child_one_index);

        void *current_address = get_element_address(array, elem_size, elem_index);

        // if child two (right) index is larger the array element count
        // we have reached the end of the array (but child one is still valid)
        size_t child_two_index = get_child_two_index(elem_index);
        if (child_two_index >= arr_max_size)
        {
            if (cmp(current_address, child_one_address))
            {
                swap(current_address, child_one_address, elem_size);
            }
            break;
        }
        void *child_two_address = get_element_address(array, elem_size, child_two_index);

        // else we choose the max child and consider swapping it with the current element
        void *max_child;
        size_t max_index;
        if (cmp(child_one_address, child_two_address))
        {
            max_child = child_two_address;
            max_index = child_two_index;
        }
        else
        {
            max_child = child_one_address;
            max_index = child_one_index;
        }

        if (cmp(current_address, max_child))
        {
            swap(current_address, max_child, elem_size);
            elem_index = max_index;
        }
        else
        {
            break;
        }
    }
}

void Logical_PQueue_Create(void *arr, size_t asize, size_t esize, size_t elemc,
        Compare_function_t cmp)
{
    if (elemc > asize)
    {
        Logical_Err_SetStatus(ERR_PQUEUE_INVALIDCOUNT);
        return;
    }

    // we use the Floyd method for building a binary heap
    // starting from the next to bottom layer, slowly go up,
    // and sift down every element
    // https://en.wikipedia.org/wiki/Binary_heap#Building_a_heap

    // get last elem of the next to bottom layer
    if (elemc == 0 || elemc == 1)
    {
        return;
    }
    size_t current_index = get_parent_index(elemc - 1);
    while (1)
    {
        if (current_index == -1u)
            break;

        sift_down(arr, elemc, esize, current_index, cmp);

        current_index--;
    }
}

void Logical_PQueue_Insert(void *arr, size_t asize, size_t esize, size_t elemc, void *elem,
        Compare_function_t cmp)
{
    if (elemc > asize)
    {
        Logical_Err_SetStatus(ERR_PQUEUE_INVALIDCOUNT);
        return;
    }
    else if (elemc == asize)
    {
        Logical_Err_SetStatus(ERR_PQUEUE_FULLINSERT);
        return;
    }

    // copy the element to the next index within the array (elemc)
    memcpy(get_element_address(arr, esize, elemc), elem, esize);
    // sift up the added element
    sift_up(arr, esize, elemc, cmp);

}

void Logical_PQueue_Pop(void *arr, size_t asize, size_t esize, size_t elemc,
        Compare_function_t cmp)
{
    if (elemc == 0)
    {
        Logical_Err_SetStatus(ERR_PQUEUE_EMPTYPOP);
        return;
    }

    // swap the last element (index elemc - 1) with the first (index 0)
    swap(get_element_address(arr, esize, 0),
            get_element_address(arr, esize, elemc - 1), esize);
    //sift down the first
    sift_down(arr, elemc - 1, esize, 0, cmp);
}

void Logical_PQueue_Delete(void *arr, size_t asize, size_t esize, size_t elemc, size_t index,
        Compare_function_t cmp)
{
    if (elemc == 0)
    {
        Logical_Err_SetStatus(ERR_PQUEUE_EMPTYPOP);
        return;
    }

    // swap the last element (index elemc - 1) with the specified element (index)
    swap(get_element_address(arr, esize, index),
            get_element_address(arr, esize, elemc - 1), esize);

    // test sift up the element
    sift_up(arr, esize, index, cmp);
    // test sift down the element
    sift_down(arr, elemc - 1, esize, index, cmp);
}

void Logical_PQueue_PushDown(void *arr, size_t esize, size_t elemc,
        Compare_function_t cmp)
{
    sift_down(arr, elemc, esize, 0, cmp);
}

