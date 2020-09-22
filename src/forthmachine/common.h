#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include "trie.h"
#include "trie.h"
#include "forthstack.h"
#include "parser.h"


struct forth_machine
{
    Trie *words;
    Trie *variables;
    ForthStack *stack;    
    char **program_words;
    size_t n_program_words;
    size_t program_counter;
};

enum error_code {
  EXECUTE_OK,
};

typedef enum error_code (*word_execution)(struct forth_machine *fmach);

#define WORD_FUNCTION_DECLARATION(name) enum error_code word_##name(struct forth_machine *fmach)


#endif