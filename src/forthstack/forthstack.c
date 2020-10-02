#include "forthstack.h"

#include <malloc.h>
#include <stdbool.h>
#include <assert.h>

struct forth_stack
{
    size_t length;
    size_t size;
    void *arr[1];
};


bool ForthStack_append(ForthStack **darr, void *val)
{
    struct forth_stack *dynarray = (struct forth_stack *) *darr;
    assert(dynarray != NULL);
    return ForthStack_insert(darr, val, dynarray->length);
}

ForthStack *ForthStack_create(size_t length)
{
    struct forth_stack *dynarray = malloc(sizeof(struct forth_stack) + sizeof(int64_t) * (length - 1));
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

bool ForthStack_insert(ForthStack **darr, void *val, size_t index)
{
    struct forth_stack *dynarray = (struct forth_stack *) *darr;
    assert(dynarray != NULL && index <= dynarray->length);
    if(dynarray->length + 1 > dynarray->size) 
    {
        struct forth_stack *realloced_dynarray = (struct forth_stack *) realloc(dynarray, sizeof(struct forth_stack) + dynarray->size * FORTHSTACK_REALLOC_FACTOR * sizeof(int64_t));
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
    struct forth_stack *realloced_dynarray = (struct forth_stack *) realloc(dynarray, sizeof(struct forth_stack) + size * sizeof(int64_t));
    if(realloced_dynarray == NULL)
        return false;

    dynarray = realloced_dynarray;
    dynarray->size = size;
    dynarray->length = dynarray->length > dynarray->size ? dynarray->size : dynarray->length;
    *darr = dynarray;
    return true;
}

void **ForthStack_arr(ForthStack *darr)
{
    struct forth_stack *dynarray = (struct forth_stack *) darr;
    assert(dynarray != NULL);   
    return dynarray->arr;
}

void *ForthStack_get(ForthStack *darr, size_t index)
{
    struct forth_stack *dynarray = (struct forth_stack *) darr;
    assert(dynarray != NULL && index < dynarray->length);   
    return dynarray->arr[index];
}

void **ForthStack_get_arr_reference(ForthStack *darr, size_t index)
{
    struct forth_stack *dynarray = (struct forth_stack *) darr;
    assert(dynarray != NULL && index < dynarray->length);   
    return &dynarray->arr[index];  
}

void ForthStack_set(ForthStack *darr, size_t index, void *val)
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

void *ForthStack_remove(ForthStack *darr, size_t index)
{
    struct forth_stack *dynarray = (struct forth_stack *) darr;
    assert(dynarray != NULL && index < dynarray->length);
    void *value = dynarray->arr[index];
    for(size_t i = index; i < dynarray->length - 1; i++)
        dynarray->arr[i] = dynarray->arr[i + 1];
    dynarray->length--;    
    return value;
}

