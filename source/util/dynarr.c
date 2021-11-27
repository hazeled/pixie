#include "dynarr.h"
#include "../config.h"
#include <malloc.h>
#include <string.h>
#include <assert.h>

dyn_arr_t _dyn_arr_create(size_t size) 
{
    dyn_arr_t ret_arr;
    ret_arr.element_size = size;
    ret_arr.length = 0;
    ret_arr.length_allocated = 2;
    ret_arr.data = malloc(size * 2);

    return ret_arr;
}

void* _dyn_arr_access(dyn_arr_t* arr, int index)
{
#ifdef DYN_ARRAY_LENGTH_CHECKS
    assert(index < arr->length);
#endif // DYN_ARRAY_LENGTH_CHECKS
    return (void*)((char*)(arr->data) + (index * arr->element_size) );
}

void _dyn_arr_push_back(dyn_arr_t* arr, void* data)
{
    if (arr->length_allocated == arr->length) {
        void* new_memory = realloc(arr->data, arr->element_size * (arr->length_allocated * 2));
        assert(new_memory != 0x00); // Will fail if OS is out of memory
        arr->data = new_memory;
        arr->length_allocated *= 2;
    }
    void* new_mem_ptr = ((char*)(arr->data) + (arr->element_size * arr->length));
    memcpy(new_mem_ptr, data, arr->element_size);
    arr->length++;
}

void _dyn_arr_pop_back(dyn_arr_t* arr)
{
#ifdef DYN_ARRAY_LENGTH_CHECKS
    assert(arr->length != 0);
#endif // DYN_ARRAY_LENGTH_CHECKS
    arr->length--;
    if (arr->length == arr->length_allocated / 2) {
        void* new_memory = realloc(arr->data, arr->element_size * (arr->length_allocated / 2));
        assert(new_memory != 0x00); // Will fail if OS is out of memory
        arr->data = new_memory;
        arr->length_allocated /= 2;
    }
}
