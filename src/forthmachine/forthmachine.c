#include "forthmachine.h"

#include <malloc.h>
#include <assert.h>

#include "trie.h"
#include "forthstack.h"

#define INITIAL_STACK_SIZE 128

struct forth_machine *forth_machine_init()
{
    struct forth_machine *fmach = (struct forth_machine *) malloc(
                                  sizeof(struct forth_machine)
                                  );
    if(fmach == NULL)
    {
        return NULL;
    }
    assert(fmach != NULL);
    Trie *words = Trie_create();
    if(words == NULL)
    {
        return NULL;
    } 
    Trie *variables = Trie_create();
    if(variables == NULL)
    {
        // Trie_destroy(words, free); need to give a different function pointer 
        return NULL;
    }
    ForthStack *stack = ForthStack_create(INITIAL_STACK_SIZE);
    if(stack == NULL)
    {
        // Trie_destroy(words, free);
        // Trie_destroy(variables, free);
        return NULL;
    }
    ForthStack_adjust_length(&stack, 0);

    fmach->words = words;
    // TODO: populate this trie  
    fmach->variables = variables;
    fmach->stack = stack;
    fmach->program_words = NULL;
    fmach->n_program_words = 0;
    fmach->program_counter = 0;
    return fmach;   
}

void forth_machine_deinit(struct forth_machine *fmach)
{
    assert(fmach != NULL);
    // Trie_destroy(fmach->words, free);
    // Trie_destroy(fmach->variables, free);
    ForthStack_destroy(fmach->stack);
    if(fmach->program_words == NULL)
    { 
        destroy_words_array(fmach->program_words, fmach->n_program_words);
    }
}

bool forth_machine_load_program(struct forth_machine *fmach, const char *program)
{
    assert(fmach != NULL && program != NULL);  
    fmach->n_program_words = num_words(program);
    fmach->program_words = make_words_array(program, fmach->n_program_words);
    fmach->program_counter = 0;
    return fmach->program_words != NULL;
}

enum error_code forth_machine_run_program(struct forth_machine);


