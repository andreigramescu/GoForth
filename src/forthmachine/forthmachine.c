#include "forthmachine.h"

#include <malloc.h>
#include <assert.h>

#include "nativewords.h"
#include "common.h"
#include "trie.h"
#include "forthstack.h"

#define INITIAL_STACK_SIZE 128

static bool init_word_trie(Trie *words);

static bool add_native_word(Trie *words, const char *word, word_execution native_function);

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
    bool init_words_success = init_word_trie(words);
    if(words == NULL || !init_words_success)
    {
        return NULL;
    } 
    Trie *variables = Trie_create();
    if(variables == NULL)
    {
        // Trie_destroy(words, free);
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
    
    ReturnStack *return_stack = ReturnStack_create(INITIAL_STACK_SIZE);
    if(return_stack == NULL)
    {
        // Trie_destroy(words, free);
        // Trie_destroy(variables, free);
        ForthStack_destroy(stack);
        return NULL;
    }
    ReturnStack_adjust_length(&return_stack, 0);

    fmach->words = words;
    // TODO: populate this trie  
    fmach->variables = variables;
    fmach->stack = stack;
    fmach->program_words = NULL;
    fmach->n_program_words = 0;
    fmach->program_counter = 0;
    fmach->return_stack = return_stack;
    return fmach;   
}

void forth_machine_deinit(struct forth_machine *fmach)
{
    assert(fmach != NULL);
    // Trie_destroy(fmach->words, free);
    // Trie_destroy(fmach->variables, free);
    ForthStack_destroy(fmach->stack);
    ReturnStack_destroy(fmach->return_stack);
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

enum error_code forth_machine_run_program(struct forth_machine *fmach)
{
    while(fmach->program_counter < fmach->n_program_words)
    {
        char *curr_word = fmach->program_words[fmach->program_counter];
        struct word_data *data;
        bool in_trie = Trie_get(fmach->words, curr_word, (void **) &data);
        if(!in_trie) 
        {
            word_not_in_trie(fmach);
        }// what if variable?
        else if(data->is_native)
        {
            enum error_code ecode = data->word_function.native_function(fmach);
            if(ecode != EXECUTE_OK) { return ecode; } 
        }
        else
        {
            ReturnStack_append(&fmach->return_stack, fmach->program_counter);
            fmach->program_counter = data->word_function.program_counter;
        }
        fmach->program_counter++;
    }
    return EXECUTE_OK;
}

static bool init_word_trie(Trie *words)
{
    bool add_success = true;
    add_success = 
        add_success && add_native_word(words, "dup", WORD_FUNCTION_ADDRESS(dup));
    add_success = 
        add_success && add_native_word(words, "drop", WORD_FUNCTION_ADDRESS(drop));
    add_success = 
        add_success && add_native_word(words, "+", WORD_FUNCTION_ADDRESS(plus));
    add_success = 
        add_success && add_native_word(words, "-", WORD_FUNCTION_ADDRESS(minus));
    add_success = 
        add_success && add_native_word(words, "*", WORD_FUNCTION_ADDRESS(star));
    add_success = 
        add_success && add_native_word(words, "/", WORD_FUNCTION_ADDRESS(slash));
    add_success = 
        add_success && add_native_word(words, "%", WORD_FUNCTION_ADDRESS(mod));
    add_success = 
        add_success && add_native_word(words, ".", WORD_FUNCTION_ADDRESS(dot));
    add_success = 
        add_success && add_native_word(words, ".s", WORD_FUNCTION_ADDRESS(dot_s));
    add_success = 
        add_success && add_native_word(words, ":", WORD_FUNCTION_ADDRESS(colon));
    add_success = 
        add_success && add_native_word(words, ";", WORD_FUNCTION_ADDRESS(semi_colon));
    return add_success;
}

static bool add_native_word(Trie *words, const char *word, word_execution native_function)
{
    struct word_data *data = (struct word_data *) malloc(sizeof(struct word_data));
    if(data == NULL)
    {
        return false;
    }
    data->is_native = true;
    data->word_function.native_function = native_function;
    return Trie_add(words, word, (void *) data);
}

