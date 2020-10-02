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

static void free_forth_variable(void *ptr);

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
    bool init_words_success = init_word_trie(words);
    if(!init_words_success) 
    {
        Trie_destroy(words, free);
        return NULL;
    }
    Trie *variables = Trie_create();
    if(variables == NULL)
    {
        Trie_destroy(words, free);
        return NULL;
    }
    ForthStack *stack = ForthStack_create(INITIAL_STACK_SIZE);
    if(stack == NULL)
    {
        Trie_destroy(words, free);
        Trie_destroy(variables, free_forth_variable);
        return NULL;
    }
    ForthStack_adjust_length(&stack, 0);
    
    ReturnStack *return_stack = ReturnStack_create(INITIAL_STACK_SIZE);
    if(return_stack == NULL)
    {
        Trie_destroy(words, free);
        Trie_destroy(variables, free_forth_variable);
        ForthStack_destroy(stack);
        return NULL;
    }
    ReturnStack_adjust_length(&return_stack, 0);

    fmach->words = words;
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
    Trie_destroy(fmach->words, free);
    Trie_destroy(fmach->variables, free_forth_variable);
    ForthStack_destroy(fmach->stack);
    ReturnStack_destroy(fmach->return_stack);
    if(fmach->program_words != NULL)
    { 
        destroy_words_array(fmach->program_words, fmach->n_program_words);
    }
    free(fmach);
}

bool forth_machine_load_program(struct forth_machine *fmach, const char *program)
{
    assert(fmach != NULL && program != NULL);  
    fmach->program_counter = fmach->n_program_words;
    size_t new_program_length = num_words(program);
    char **new_program_parsed = make_words_array(program, new_program_length); 
    size_t new_size;
    fmach->program_words = append_words_array(fmach->program_words, fmach->n_program_words, new_program_parsed, new_program_length, &new_size); 
    fmach->n_program_words = new_size;
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
            enum error_code ecode = word_not_in_trie(fmach);
            if(ecode != EXECUTE_OK) { return ecode; } 
        }// what if variable?
        else if(data->is_native)
        {
            enum error_code ecode = data->word_function.native_function(fmach);
            if(ecode != EXECUTE_OK) { return ecode; } 
        }
        else
        {
            bool is_appended = 
                ReturnStack_append(&fmach->return_stack, fmach->program_counter);
            if(!is_appended) { return STACK_RESIZE_FAIL; }
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
    add_success = 
        add_success && add_native_word(words, "variable", WORD_FUNCTION_ADDRESS(variable));
     add_success = 
        add_success && add_native_word(words, "array", WORD_FUNCTION_ADDRESS(array));
     add_success = 
        add_success && add_native_word(words, "!", WORD_FUNCTION_ADDRESS(exclamation));
     add_success = 
        add_success && add_native_word(words, "if", WORD_FUNCTION_ADDRESS(if));
     add_success = 
        add_success && add_native_word(words, "then", WORD_FUNCTION_ADDRESS(then));
     add_success = 
        add_success && add_native_word(words, "else", WORD_FUNCTION_ADDRESS(else));
     add_success = 
        add_success && add_native_word(words, "@", WORD_FUNCTION_ADDRESS(fetch));

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

static void free_forth_variable(void *ptr)
{
    struct forth_variable *var = (struct forth_variable *) ptr;
    if(var != NULL)
    {
        if(var->type == FORTH_VARIABLE_ARR)
        {
            free(var->data.arr); 
        }
        free(var);
    }
}

