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
  UNDEFINED_WORD,
  UNKNOWN_VARIABLE,
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
