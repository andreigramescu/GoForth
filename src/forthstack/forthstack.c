#include "forthstack.h"

#include <malloc.h>
#include <stdbool.h>
#include <assert.h>

struct forth_stack
{
    size_t length;
    size_t size;
    uint64_t arr[1];
};


bool ForthStack_append(ForthStack **darr, uint64_t val)
{
    struct forth_stack *dynarray = (struct forth_stack *) *darr;
    assert(dynarray != NULL);
    return ForthStack_insert(darr, val, dynarray->length);
}

ForthStack *ForthStack_create(size_t length)
{
    struct forth_stack *dynarray = malloc(sizeof(struct forth_stack) + sizeof(uint64_t) * (length - 1));
    if(dynarray == NULL)
        return NULL;

    dynarray->length = length;
    dynarray->size   = length;
    return dynarray;
}

void ForthStack_destroy(ForthStack *dynarray)
{
    free(dynarray);
}

bool ForthStack_insert(ForthStack **darr, uint64_t val, size_t index)
{
    struct forth_stack *dynarray = (struct forth_stack *) *darr;
    assert(dynarray != NULL && index <= dynarray->length);
    if(dynarray->length + 1 > dynarray->size) 
    {
        struct forth_stack *realloced_dynarray = (struct forth_stack *) realloc(dynarray, sizeof(struct forth_stack) + dynarray->size * FORTHSTACK_REALLOC_FACTOR * sizeof(uint64_t));
        if(realloced_dynarray == NULL)
            return false;

        dynarray = realloced_dynarray;
        dynarray->size *= FORTHSTACK_REALLOC_FACTOR;
        *darr = (void *) dynarray;
    } 
    size_t curr = dynarray->length;
    while(curr != index)
    {
       dynarray->arr[curr] = dynarray->arr[curr - 1]; 
       curr--;
    } 
    dynarray->arr[curr] = val;
    dynarray->length++;
    return true;
}

size_t ForthStack_length(ForthStack *darr)
{
    struct forth_stack *dynarray = (struct forth_stack *) darr;
    assert(dynarray != NULL); 
    return dynarray->length;
}

size_t ForthStack_size(ForthStack *darr)
{
    struct forth_stack *dynarray = (struct forth_stack *) darr;
    assert(dynarray != NULL); 
    return dynarray->size;
}


bool ForthStack_resize(ForthStack **darr, size_t size)
{
    struct forth_stack *dynarray = (struct forth_stack *) *darr;
    assert(dynarray != NULL); 
    struct forth_stack *realloced_dynarray = (struct forth_stack *) realloc(dynarray, sizeof(struct forth_stack) + size * sizeof(uint64_t));
    if(realloced_dynarray == NULL)
        return false;

    dynarray = realloced_dynarray;
    dynarray->size = size;
    dynarray->length = dynarray->length > dynarray->size ? dynarray->size : dynarray->length;
    *darr = dynarray;
    return true;
}

uint64_t *ForthStack_arr(ForthStack *darr)
{
    struct forth_stack *dynarray = (struct forth_stack *) darr;
    assert(dynarray != NULL);   
    return dynarray->arr;
}

uint64_t ForthStack_get(ForthStack *darr, size_t index)
{
    struct forth_stack *dynarray = (struct forth_stack *) darr;
    assert(dynarray != NULL && index < dynarray->length);   
    return dynarray->arr[index];
}

uint64_t *ForthStack_get_arr_reference(ForthStack *darr, size_t index)
{
    struct forth_stack *dynarray = (struct forth_stack *) darr;
    assert(dynarray != NULL && index < dynarray->length);   
    return &dynarray->arr[index];  
}

void ForthStack_set(ForthStack *darr, size_t index, uint64_t val)
{
    struct forth_stack *dynarray = (struct forth_stack *) darr;
    assert(dynarray != NULL && index < dynarray->length);   
    dynarray->arr[index] = val;
}

bool ForthStack_adjust_length(ForthStack **darr, size_t length)
{
    struct forth_stack *dynarray = (struct forth_stack *) *darr;
    if(length > dynarray->size)
    {
        if(!ForthStack_resize(darr, length))
        {
            return false;
        }
        dynarray = (struct forth_stack *) *darr;
    }
    dynarray->length = length;
    return true;
}

bool ForthStack_insert_array(ForthStack **darr, size_t start_index, uint64_t *arr, size_t arr_length)
{
    struct forth_stack *dynarray = (struct forth_stack *) *darr;
    assert(dynarray != NULL && start_index <= dynarray->length);
    size_t old_length = dynarray->length; 
    if(!ForthStack_adjust_length(darr, dynarray->length + arr_length))
        return false;

    dynarray = (struct forth_stack *) *darr;
    size_t i = old_length;
    while(i > start_index)
    {
        i--;
        dynarray->arr[i + arr_length] = dynarray->arr[i];
    }        
    // we know i = start_index
    for(; i < start_index + arr_length; i++)
        dynarray->arr[i] = arr[i - start_index];

    return true;
}

bool ForthStack_append_array(ForthStack **darr, uint64_t *arr, size_t arr_length)
{
    struct forth_stack *dynarray = (struct forth_stack *) *darr;
    assert(dynarray != NULL);
    return ForthStack_insert_array(darr, dynarray->length, arr, arr_length);     
}

bool ForthStack_insert_dynarray(ForthStack **dest, size_t start_index, ForthStack *src)
{
    struct forth_stack *dynarray_src = (struct forth_stack *) src;
    assert(dynarray_src != NULL);
    return ForthStack_insert_array(dest, start_index, dynarray_src->arr, dynarray_src->length); 
}

bool ForthStack_append_dynarray(ForthStack **dest, ForthStack *src)
{
    struct forth_stack *dynarray_src = (struct forth_stack *) src;
    assert(dynarray_src != NULL);
    return ForthStack_append_array(dest, dynarray_src->arr, dynarray_src->length); 
}

void ForthStack_remove(ForthStack *darr, size_t index)
{
    struct forth_stack *dynarray = (struct forth_stack *) darr;
    assert(dynarray != NULL && index < dynarray->length);
    for(size_t i = index; i < dynarray->length - 1; i++)
        dynarray->arr[i] = dynarray->arr[i + 1];
    dynarray->length--;    
}

void ForthStack_remove_slice(ForthStack *darr, size_t start, size_t end)
{
    struct forth_stack *dynarray = (struct forth_stack *) darr;
    assert(dynarray != NULL && start < dynarray->length &&
            end <= dynarray->length && end > start);
    size_t slice_size = end - start;
    size_t i;
    for(i = start; i < start + (dynarray->length - end); i++)
    {
        dynarray->arr[i] = dynarray->arr[i + slice_size];
    }
    dynarray->length = i;
}

void ForthStack_foreach(ForthStack *darr, ForthStack_foreach_fp fp)
{
    struct forth_stack *dynarray = (struct forth_stack *) darr;
    assert(dynarray != NULL);  
    uint64_t *arr = dynarray->arr;
    for(size_t i = 0; i < dynarray->length; i++)
    {
        fp(arr[i]);
    }
}
