#ifndef FORTHSTACK_H
#define FORTHSTACK_H

#ifndef FORTHSTACK_REALLOC_FACTOR
#define FORTHSTACK_REALLOC_FACTOR 2
#endif

#include <stdbool.h>
#include <malloc.h>
#include <stdint.h>

typedef void ForthStack;

// adds val to the end of the arr. true on success
bool ForthStack_append(ForthStack **darr, void *val);
// creates a new dynarray with length and size of length
ForthStack *ForthStack_create(size_t length);
// deallocates all memory used my dynarray
void ForthStack_destroy(ForthStack *darr);
// inserts a value into the dynarray. true on success
bool ForthStack_insert(ForthStack **darr, void *val, size_t index);
// gets length of dynarray
size_t ForthStack_length(ForthStack *darr);
// gets size of dynarray
size_t ForthStack_size(ForthStack *darr);
// changes the size that the dynarray takes. true on success
bool ForthStack_resize(ForthStack **darr, size_t size);
// gets the array data of the dynarray
void **ForthStack_arr(ForthStack *darr);
// gets a value in the array at a particular index
void *ForthStack_get(ForthStack *darr, size_t index);
// gets the reference of the data in the array
void **ForthStack_get_arr_reference(ForthStack *darr, size_t index);
// sets a value in the array at a particular index
void ForthStack_set(ForthStack *darr, size_t index, void *val);
// changes length of the array. true on success
bool ForthStack_adjust_length(ForthStack **darr, size_t length);
// removes an element from dynarray
void *ForthStack_remove(ForthStack *darr, size_t index);

#endif
