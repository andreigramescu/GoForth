#include "ReturnStack.h"

#include <malloc.h>
#include <stdbool.h>
#include <assert.h>

struct return_stack
{
    size_t length;
    size_t size;
    size_t arr[1];
};


bool ReturnStack_append(ReturnStack **darr, size_t val)
{
    struct return_stack *dynarray = (struct return_stack *) *darr;
    assert(dynarray != NULL);
    return ReturnStack_insert(darr, val, dynarray->length);
}

ReturnStack *ReturnStack_create(size_t length)
{
    struct return_stack *dynarray = malloc(sizeof(struct return_stack) + sizeof(size_t) * (length - 1));
    if(dynarray == NULL)
        return NULL;

    dynarray->length = length;
    dynarray->size   = length;
    return dynarray;
}

void ReturnStack_destroy(ReturnStack *dynarray)
{
    free(dynarray);
}

bool ReturnStack_insert(ReturnStack **darr, size_t val, size_t index)
{
    struct return_stack *dynarray = (struct return_stack *) *darr;
    assert(dynarray != NULL && index <= dynarray->length);
    if(dynarray->length + 1 > dynarray->size) 
    {
        struct return_stack *realloced_dynarray = (struct return_stack *) realloc(dynarray, sizeof(struct return_stack) + dynarray->size * RETURNSTACK_REALLOC_FACTOR * sizeof(size_t));
        if(realloced_dynarray == NULL)
            return false;

        dynarray = realloced_dynarray;
        dynarray->size *= RETURNSTACK_REALLOC_FACTOR;
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

size_t ReturnStack_length(ReturnStack *darr)
{
    struct return_stack *dynarray = (struct return_stack *) darr;
    assert(dynarray != NULL); 
    return dynarray->length;
}

size_t ReturnStack_size(ReturnStack *darr)
{
    struct return_stack *dynarray = (struct return_stack *) darr;
    assert(dynarray != NULL); 
    return dynarray->size;
}


bool ReturnStack_resize(ReturnStack **darr, size_t size)
{
    struct return_stack *dynarray = (struct return_stack *) *darr;
    assert(dynarray != NULL); 
    struct return_stack *realloced_dynarray = (struct return_stack *) realloc(dynarray, sizeof(struct return_stack) + size * sizeof(size_t));
    if(realloced_dynarray == NULL)
        return false;

    dynarray = realloced_dynarray;
    dynarray->size = size;
    dynarray->length = dynarray->length > dynarray->size ? dynarray->size : dynarray->length;
    *darr = dynarray;
    return true;
}

size_t *ReturnStack_arr(ReturnStack *darr)
{
    struct return_stack *dynarray = (struct return_stack *) darr;
    assert(dynarray != NULL);   
    return dynarray->arr;
}

size_t ReturnStack_get(ReturnStack *darr, size_t index)
{
    struct return_stack *dynarray = (struct return_stack *) darr;
    assert(dynarray != NULL && index < dynarray->length);   
    return dynarray->arr[index];
}

size_t *ReturnStack_get_arr_reference(ReturnStack *darr, size_t index)
{
    struct return_stack *dynarray = (struct return_stack *) darr;
    assert(dynarray != NULL && index < dynarray->length);   
    return &dynarray->arr[index];  
}

void ReturnStack_set(ReturnStack *darr, size_t index, size_t val)
{
    struct return_stack *dynarray = (struct return_stack *) darr;
    assert(dynarray != NULL && index < dynarray->length);   
    dynarray->arr[index] = val;
}

bool ReturnStack_adjust_length(ReturnStack **darr, size_t length)
{
    struct return_stack *dynarray = (struct return_stack *) *darr;
    if(length > dynarray->size)
    {
        if(!ReturnStack_resize(darr, length))
        {
            return false;
        }
        dynarray = (struct return_stack *) *darr;
    }
    dynarray->length = length;
    return true;
}

bool ReturnStack_insert_array(ReturnStack **darr, size_t start_index, size_t *arr, size_t arr_length)
{
    struct return_stack *dynarray = (struct return_stack *) *darr;
    assert(dynarray != NULL && start_index <= dynarray->length);
    size_t old_length = dynarray->length; 
    if(!ReturnStack_adjust_length(darr, dynarray->length + arr_length))
        return false;

    dynarray = (struct return_stack *) *darr;
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

bool ReturnStack_append_array(ReturnStack **darr, size_t *arr, size_t arr_length)
{
    struct return_stack *dynarray = (struct return_stack *) *darr;
    assert(dynarray != NULL);
    return ReturnStack_insert_array(darr, dynarray->length, arr, arr_length);     
}

bool ReturnStack_insert_dynarray(ReturnStack **dest, size_t start_index, ReturnStack *src)
{
    struct return_stack *dynarray_src = (struct return_stack *) src;
    assert(dynarray_src != NULL);
    return ReturnStack_insert_array(dest, start_index, dynarray_src->arr, dynarray_src->length); 
}

bool ReturnStack_append_dynarray(ReturnStack **dest, ReturnStack *src)
{
    struct return_stack *dynarray_src = (struct return_stack *) src;
    assert(dynarray_src != NULL);
    return ReturnStack_append_array(dest, dynarray_src->arr, dynarray_src->length); 
}

size_t ReturnStack_remove(ReturnStack *darr, size_t index)
{
    struct return_stack *dynarray = (struct return_stack *) darr;
    assert(dynarray != NULL && index < dynarray->length);
    size_t value = dynarray->arr[index]; 
    for(size_t i = index; i < dynarray->length - 1; i++)
        dynarray->arr[i] = dynarray->arr[i + 1];
    dynarray->length--;    
    return value; 
}

void ReturnStack_remove_slice(ReturnStack *darr, size_t start, size_t end)
{
    struct return_stack *dynarray = (struct return_stack *) darr;
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

void ReturnStack_foreach(ReturnStack *darr, ReturnStack_foreach_fp fp)
{
    struct return_stack *dynarray = (struct return_stack *) darr;
    assert(dynarray != NULL);  
    size_t *arr = dynarray->arr;
    for(size_t i = 0; i < dynarray->length; i++)
    {
        fp(arr[i]);
    }
}
