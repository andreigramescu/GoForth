#include <stdbool.h>
#include <assert.h>
#include <inttypes.h>
#include <string.h>

#include "nativewords.h"
#include "forthmachine.h"
#include "forthstack.h"


static char *get_curr_word(struct forth_machine *fmach) {
  return fmach->program_words[fmach->program_counter];
}

enum error_code word_number(struct forth_machine *fmach) {
  char *endptr;
  int64_t val = strtoimax(get_curr_word(fmach), &endptr, 0);
  ForthStack_append(&(fmach->stack), (void *) val);
  return EXECUTE_OK;
 }

enum error_code word_dup(struct forth_machine *fmach) {
  if (ForthStack_length(fmach->stack) == 0) {
    return STACK_EMPTY;
  }
  ForthStack_append(&(fmach->stack), ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1));
  return EXECUTE_OK;
}

enum error_code word_drop(struct forth_machine *fmach) {
  if (ForthStack_length(fmach->stack) == 0) {
    return STACK_EMPTY;
  }
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  return EXECUTE_OK;
}

enum error_code word_plus(struct forth_machine *fmach) {
  size_t length = ForthStack_length(fmach->stack);
  if (length == 0 || length == 1) {
    return STACK_EMPTY;
  }
  int64_t top1 = (int64_t) ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  int64_t top2 = (int64_t) ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_append(&(fmach->stack), (void *) (top1 + top2));
  return EXECUTE_OK;
}

enum error_code word_minus(struct forth_machine *fmach) {
  size_t length = ForthStack_length(fmach->stack);
  if (length == 0 || length == 1) {
    return STACK_EMPTY;
  }
  int64_t top1 = (int64_t) ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  int64_t top2 = (int64_t) ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_append(&(fmach->stack), (void *) (top2 - top1));
  return EXECUTE_OK;
}


enum error_code word_star(struct forth_machine *fmach) {
  size_t length = ForthStack_length(fmach->stack);
  if (length == 0 || length == 1) {
    return STACK_EMPTY;
  }
  int64_t top1 = (int64_t) ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  int64_t top2 = (int64_t) ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_append(&(fmach->stack), (void *) (top1 * top2));
  return EXECUTE_OK;
}

enum error_code word_slash(struct forth_machine *fmach) {
  size_t length = ForthStack_length(fmach->stack);
  if (length == 0 || length == 1) {
    return STACK_EMPTY;
  }
  int64_t top1 = (int64_t) ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  int64_t top2 = (int64_t) ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_append(&(fmach->stack), (void *) (top2 / top1));
  return EXECUTE_OK;
}

enum error_code word_mod(struct forth_machine *fmach) {
  size_t length = ForthStack_length(fmach->stack);
  if (length == 0 || length == 1) {
    return STACK_EMPTY;
  }
  int64_t top1 = (int64_t) ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  int64_t top2 = (int64_t) ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_append(&(fmach->stack), (void *) (top2 % top1));
  return EXECUTE_OK;
}

enum error_code word_dot(struct forth_machine *fmach) {
  printf("%ld\n", (int64_t) ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1));
  return EXECUTE_OK;
}

enum error_code word_dot_s(struct forth_machine *fmach) {
  if (ForthStack_length(fmach->stack) == 0) {
    puts("Stack empty");
  } else {
    for (int i = 0; i < ForthStack_length(fmach->stack); i++) {
      printf("%ld ", (int64_t) ForthStack_get(fmach->stack, i));
    }
    putchar('\n');
  }
  return EXECUTE_OK;
}

enum error_code word_not_in_trie(struct forth_machine *fmach) {
  
  char *curr_word = get_curr_word(fmach);
  struct forth_variable *fdata;
  bool in_trie = Trie_get(fmach->variables, curr_word, (void **) &fdata);
  if (in_trie) {
    bool append_ok = false;
    switch (fdata->type) {
      case FORTH_VARIABLE_VALUE: { 
        append_ok = ForthStack_append(&(fmach->stack), (void *) &(fdata->data.variable));
        if (!append_ok) {
          return STACK_RESIZE_FAIL;
        }
        return EXECUTE_OK;
      }
      case FORTH_VARIABLE_ARR: {
        append_ok = ForthStack_append(&(fmach->stack), (void *) fdata->data.arr);
        if (!append_ok) {
          return STACK_RESIZE_FAIL;
        }
        return EXECUTE_OK;
      }
      default: {  
        assert(false);
        return MALFORMED_VARIABLE_TRIE; 
      }
    }
  } else {
      return word_number(fmach);
  }
}

enum error_code word_colon(struct forth_machine *fmach) {
    fmach->program_counter++;
    if(fmach->program_counter >= fmach->n_program_words) {
        return WORD_DEFINITION_UNTERMINATED; 
    }
    struct word_data *data = (struct word_data *) malloc(sizeof(struct word_data));
    data->is_native = false;
    data->word_function.program_counter = fmach->program_counter;
    /* TODO: consider if a word is redifined, the void * 
    ** will be changed and a memory leak will be caused! fix l8r */
    bool add_success = Trie_add(
                        fmach->words,
                        fmach->program_words[fmach->program_counter], 
                        (void *) data);
    if(!add_success) {
        free(data);
        return WORD_PREV_DEFINED;
    }

    do {
        fmach->program_counter++;
        if(fmach->program_counter >= fmach->n_program_words) {
            return WORD_DEFINITION_UNTERMINATED; 
        }
    } while(strcmp(fmach->program_words[fmach->program_counter], ";") != 0);
    return EXECUTE_OK; 
}

enum error_code word_semi_colon(struct forth_machine *fmach) {
    size_t rstack_length = ReturnStack_length(fmach->return_stack);
    if(rstack_length == 0) {
         return SEMI_COLON_BEFORE_DEFINITION; 
    }
    fmach->program_counter = ReturnStack_remove(fmach->return_stack, rstack_length - 1);  
    
    return EXECUTE_OK;
}

enum error_code word_variable(struct forth_machine *fmach) {
    if (++fmach->program_counter >= fmach->n_words) {
      return TOO_FEW_PARAMS; 
    }
    char *next_word = fmach->program_words[fmach->program_counter];  
    void *dummy;
    bool in_tree = Trie_get(fmach->words, next_word, &dummy);
    if (in_tree) {
      return WORD_PREV_DEFINED;
    }
    struct forth_variable *value = malloc(sizeof(struct forth_value));
    if (value == NULL) {
      return MEMORY_ERROR;
    }
    value->type = FORTH_VARIABLE_VALUE;
    in_tree = Trie_add(fmach->words, next_word, value);
    if (!in_tree) {
      return VAR_PREV_DEFINED; 
    }
    return EXECUTE_OK;
}
