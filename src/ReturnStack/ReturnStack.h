#ifndef RETURNSTACK_H
#define RETURNSTACK_H

#ifndef RETURNSTACK_REALLOC_FACTOR
#define RETURNSTACK_REALLOC_FACTOR 2
#endif

#include <stdbool.h>
#include <malloc.h>

typedef void ReturnStack;

// adds val to the end of the arr. true on success
bool ReturnStack_append(ReturnStack **darr, size_t val);
// creates a new dynarray with length and size of length
ReturnStack *ReturnStack_create(size_t length);
// deallocates all memory used my dynarray
void ReturnStack_destroy(ReturnStack *darr);
// inserts a value into the dynarray. true on success
bool ReturnStack_insert(ReturnStack **darr, size_t val, size_t index);
// gets length of dynarray
size_t ReturnStack_length(ReturnStack *darr);
// gets size of dynarray
size_t ReturnStack_size(ReturnStack *darr);
// changes the size that the dynarray takes. true on success
bool ReturnStack_resize(ReturnStack **darr, size_t size);
// gets the array data of the dynarray
size_t *ReturnStack_arr(ReturnStack *darr);
// gets a value in the array at a particular index
size_t ReturnStack_get(ReturnStack *darr, size_t index);
// gets the reference of the data in the array
size_t *ReturnStack_get_arr_reference(ReturnStack *darr, size_t index);
// sets a value in the array at a particular index
void ReturnStack_set(ReturnStack *darr, size_t index, size_t val);
// changes length of the array. true on success
bool ReturnStack_adjust_length(ReturnStack **darr, size_t length);
// removes an element from dynarray
size_t ReturnStack_remove(ReturnStack *darr, size_t index);
#endif
