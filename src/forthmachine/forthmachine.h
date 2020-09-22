#ifndef FORTHMACHINE_H
#define FORTHMACHINE_H 

#include <stdbool.h>

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

bool ForthMachine_init(struct forth_machine *fmach);

void ForthMachine_deinit(struct forth_machine *fmach);

bool ForthMachine_load_program(struct forth_machine *fmach, const char *program);

enum error_code ForthMachine_run_program(struct forth_machine);

#endif /* FORTHMACHINE_H */
