#include "forthmachine.h"

#include <malloc.h>
#include <assert.h>

#include "trie.h"
#include "forthstack.h"

#define INITIAL_STACK_SIZE 128

bool ForthMachine_init(struct forth_machine *fmach)
{
    assert(fmach != NULL);
    Trie *words = Trie_create();
    if(words == NULL)
    {
        return false;
    } 
    Trie *variables = Trie_create();
    if(variables == NULL)
    {
        Trie_destroy(words, free);
        return false;
    }
    ForthStack *stack = ForthStack_create(INITIAL_STACK_SIZE);
    if(stack == NULL)
    {
        Trie_destroy(words, free);
        Trie_destroy(variables, free);
        return false;
    }
    fmach->words = words;
    fmach->variables = variables;
    fmach->stack = stack;
    fmach->program_words = NULL;
    fmach->n_program_words = 0;
    fmach->program_words = 0;
    return true;   
}

void ForthMachine_deinit(struct forth_machine *fmach)
{
    assert(fmach != NULL);
    Trie_destroy(fmach->words, free);
    Trie_destroy(fmach->variables, free);
    ForthStack_destroy(fmach->stack);
    if(fmach->program_words == NULL)
    { 
        destroy_words_array(fmach->program_words, fmach->n_program_words);
    }
}



