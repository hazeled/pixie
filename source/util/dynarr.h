#pragma once 

#include <stdint.h>
#include <stdlib.h>

// A dynamic array is a continous region in memory pointed at by dyn_array_t 
// This is to allow for O(1) complexity for dyn_array_pop_back, dyn_array_pop_front, and dyn_array_push_back
// A dynamic array will double in size every time it needs to expand, and half in size when shrinking

#define DYN_ARR_CREATE(type) _dyn_arr_create(sizeof(type))                           // Create a new dynamic array

#define DYN_ARR_ACCESS(arr, type, index) *(type*)(_dyn_arr_access(&arr, index))    // Get item at index
#define DYN_ARR_ACCESS_PTR(arr, type, index) (*type)_dyn_arr_access(&arr, index) // Get pointer to item at index
#define DYN_ARR_PUSH_BACK(arr, data) _dyn_arr_push_back(&arr, &data)

#define DYN_ARR_POP_BACK(arr) _dyn_arr_pop_back(&arr)

typedef struct dyn_array_t {
    void* data;
    size_t element_size;
    uint32_t length;
    uint32_t length_allocated;
} dyn_arr_t;

dyn_arr_t _dyn_arr_create(size_t size);

void* _dyn_arr_access(dyn_arr_t* arr, int index);
void _dyn_arr_push_back(dyn_arr_t* arr, void* data);

void _dyn_arr_pop_back(dyn_arr_t* arr);
