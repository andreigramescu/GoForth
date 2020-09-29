#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <stddef.h>

#include "trie.h"
#include "trie.h"
#include "forthstack.h"
#include "parser.h"
#include "ReturnStack.h"

struct forth_machine
{
    Trie *words;
    Trie *variables;
    ForthStack *stack;    
    char **program_words;
    size_t n_program_words;
    size_t program_counter;
    ReturnStack *return_stack;
};

enum error_code {
  EXECUTE_OK,
  STACK_EMPTY,
  STACK_RESIZE_FAIL,
  UNDEFINED_WORD,
  UNKNOWN_VARIABLE,
  MALFORMED_VARIABLE_TRIE,
  WORD_DEFINITION_UNTERMINATED, 
  SEMI_COLON_BEFORE_DEFINITION,
};

enum forth_variable_type {
    FORTH_VARIABLE_VALUE,
    FORTH_VARIABLE_ARR,
};

struct forth_variable {
    union {
        int64_t variable;
        int64_t *arr;
    } data;
    enum forth_variable_type type;
};

typedef enum error_code (*word_execution)(struct forth_machine *fmach);

struct word_data
{
    union
    {
        word_execution native_function;
        size_t program_counter;
    } word_function;
    bool is_native;
};

#define WORD_FUNCTION_DECLARATION(name)\
    enum error_code word_##name(struct forth_machine *fmach)

#define WORD_FUNCTION_ADDRESS(name)\
    word_##name

#endif
