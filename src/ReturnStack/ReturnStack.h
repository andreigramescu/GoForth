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
// insert array at an index
bool ReturnStack_insert_array(ReturnStack **darr, size_t start_index, size_t *arr, size_t arr_length);
// appends array. just a wrapper for ReturnStack_insert_array
bool ReturnStack_append_array(ReturnStack **darr, size_t *arr, size_t arr_length); 
// inserts a dynarray into another dynarray
bool ReturnStack_insert_dynarray(ReturnStack **dest, size_t start_index, ReturnStack *src);
// appends a dynarray -tests not done
bool ReturnStack_append_dynarray(ReturnStack **dest, ReturnStack *src);
// removes an element from dynarray
void ReturnStack_remove(ReturnStack *darr, size_t index);
// removes slice of array. it removes elements from start up to but not including end 
void ReturnStack_remove_slice(ReturnStack *darr, size_t start, size_t end);
// executes fp on each element up to the length of the dynarray
typedef void (*ReturnStack_foreach_fp)(size_t element);
void ReturnStack_foreach(ReturnStack *darr, ReturnStack_foreach_fp fp);

#endif
