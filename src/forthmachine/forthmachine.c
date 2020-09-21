#include "forthmachine.h"

#include <malloc.h>

#include "trie.h"
#include "forthstack.h"

struct forth_machine
{
    Trie *words;
    Trie *variables;
    ForthStack stack;    
    char **program_words;
    size_t n_program_words;
    size_t program_counter;
};

ForthMachine *ForthMachine_init(void);
