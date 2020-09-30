#ifndef STRING_H
#define STRING_H

#ifndef STRING_REALLOC_FACTOR
#define STRING_REALLOC_FACTOR 2
#endif

#include <stdbool.h>
#include <malloc.h>

typedef void String;

// adds val to the end of the arr. true on success
bool String_append(String **darr, char val);
// creates a new dynarray with length and size of length
String *String_create(size_t length);
// deallocates all memory used my dynarray
void String_destroy(String *darr);
// inserts a value into the dynarray. true on success
bool String_insert(String **darr, char val, size_t index);
// gets length of dynarray
size_t String_length(String *darr);
// gets size of dynarray
size_t String_size(String *darr);
// changes the size that the dynarray takes. true on success
bool String_resize(String **darr, size_t size);
// gets the array data of the dynarray
char *String_arr(String *darr);
// gets a value in the array at a particular index
char String_get(String *darr, size_t index);
// gets the reference of the data in the array
char *String_get_arr_reference(String *darr, size_t index);
// sets a value in the array at a particular index
void String_set(String *darr, size_t index, char val);
// changes length of the array. true on success
bool String_adjust_length(String **darr, size_t length);
// insert array at an index
bool String_insert_array(String **darr, size_t start_index, char *arr, size_t arr_length);
// appends array. just a wrapper for String_insert_array
bool String_append_array(String **darr, char *arr, size_t arr_length); 
// inserts a dynarray into another dynarray
bool String_insert_dynarray(String **dest, size_t start_index, String *src);
// appends a dynarray -tests not done
bool String_append_dynarray(String **dest, String *src);
// removes an element from dynarray
void String_remove(String *darr, size_t index);
// removes slice of array. it removes elements from start up to but not including end 
void String_remove_slice(String *darr, size_t start, size_t end);
// executes fp on each element up to the length of the dynarray
typedef void (*String_foreach_fp)(char element);
void String_foreach(String *darr, String_foreach_fp fp);

#endif
