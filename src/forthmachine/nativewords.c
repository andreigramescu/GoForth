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
  if(*endptr != '\0') {
    return UNDEFINED_WORD;
  }
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
    printf("<%lu> ", ForthStack_length(fmach->stack));
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
    if (++fmach->program_counter >= fmach->n_program_words) {
      return TOO_FEW_PARAMS; 
    }
    char *next_word = fmach->program_words[fmach->program_counter];  
    void *dummy;
    bool in_tree = Trie_get(fmach->words, next_word, &dummy);
    if (in_tree) {
      return WORD_PREV_DEFINED;
    }
    struct forth_variable *value = malloc(sizeof(struct forth_variable));
    if (value == NULL) {
      return MEMORY_ERROR;
    }
    value->type = FORTH_VARIABLE_VALUE;
    in_tree = Trie_add(fmach->variables, next_word, value);
    if (!in_tree) {
      return VAR_PREV_DEFINED; 
    }
    return EXECUTE_OK;
}

enum error_code word_array(struct forth_machine *fmach) {
    if(fmach->program_counter + 2 >= fmach->n_program_words) {
        return TOO_FEW_PARAMS;
    }  
    char *name = fmach->program_words[fmach->program_counter + 1];
    char *str_size = fmach->program_words[fmach->program_counter + 2];
    char *endptr;
    size_t size = strtoumax(str_size, &endptr, 10);     
    if(*endptr != '\0') {
        return NOT_A_UINT;
    }   

    void *dummy;
    bool in_tree = Trie_get(fmach->words, name, &dummy);
    if (in_tree) {
      return WORD_PREV_DEFINED;
    }

    struct forth_variable *array = (struct forth_variable *) 
                                   malloc(sizeof(struct forth_variable));
    array->type = FORTH_VARIABLE_ARR;
    array->data.arr = (int64_t *) malloc(sizeof(int64_t) * size);
    bool add_success = Trie_add(fmach->variables, name, array);
    if(!add_success) {
        free(array->data.arr);
        free(array);
        return VAR_PREV_DEFINED; 
    }
    
    return EXECUTE_OK;   
}

enum error_code word_exclamation(struct forth_machine *fmach) {
    if(ForthStack_length(fmach->stack) <= 1) {
        return STACK_EMPTY; 
    }
    int64_t *address = (int64_t *) ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
    int64_t value = (int64_t) ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
    *address = value;
    return EXECUTE_OK;
}

enum error_code word_if(struct forth_machine *fmach) {
  if (ForthStack_length(fmach->stack) == 0) {
    return STACK_EMPTY;
  }
  if (!ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1)) {
    for (size_t i = fmach->program_counter; i < fmach->n_program_words && strcmp(fmach->program_words[i], "then"); i++) {
      fmach->program_counter++;
    }
  }
  return EXECUTE_OK;
}

enum error_code word_then(struct forth_machine *fmach) {
    for (size_t i = fmach->program_counter; i < fmach->n_program_words && strcmp(fmach->program_words[i], "else"); i++) {
      fmach->program_counter++;
    }
    return EXECUTE_OK;
}

enum error_code word_else(struct forth_machine *fmach) {/*does nothing*/return EXECUTE_OK;}

enum error_code word_fetch(struct forth_machine *fmach) {
    void *ptr = ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
    printf("%ld\n", *((int64_t *) ptr));
    return EXECUTE_OK;
}

enum error_code word_rot(struct forth_machine *fmach) {
    size_t length = ForthStack_length(fmach->stack);
    if (length < 3) {
        return STACK_EMPTY;
    }
    void **arr = ForthStack_arr(fmach->stack);
    void *hold = arr[length - 1];
    arr[length - 1] = arr[length - 2];
    arr[length - 2] = arr[length - 3];
    arr[length - 3] = hold;
    return EXECUTE_OK;
}

enum error_code word_swap(struct forth_machine *fmach) {
    size_t length = ForthStack_length(fmach->stack);
    if (length < 2) {
        return STACK_EMPTY;
    }
    void **arr = ForthStack_arr(fmach->stack);
    void *hold = arr[length - 2];
    arr[length - 2] = arr[length - 1];
    arr[length - 1] = hold; 
    return EXECUTE_OK;
}

enum error_code word_less(struct forth_machine *fmach) {
    size_t length = ForthStack_length(fmach->stack);
    if (length < 2) {
        return STACK_EMPTY;
    }
    int64_t first = (int64_t) ForthStack_remove(fmach->stack, length - 1);
    int64_t second = (int64_t) ForthStack_remove(fmach->stack, length - 2);
    ForthStack_append(&fmach->stack, (void *) (second < first ? (int64_t) -1 : (int64_t) 0));
    return EXECUTE_OK;
}

enum error_code word_greater(struct forth_machine *fmach) {
    size_t length = ForthStack_length(fmach->stack);
    if (length < 2) {
        return STACK_EMPTY;
    }
    int64_t first = (int64_t) ForthStack_remove(fmach->stack, length - 1);
    int64_t second = (int64_t) ForthStack_remove(fmach->stack, length - 2);
    ForthStack_append(&fmach->stack, (void *) (second > first ? (int64_t) -1 : (int64_t) 0));
    return EXECUTE_OK;
}

enum error_code word_equal(struct forth_machine *fmach) {
    size_t length = ForthStack_length(fmach->stack);
    if (length < 2) {
        return STACK_EMPTY;
    }
    int64_t first = (int64_t) ForthStack_remove(fmach->stack, length - 1);
    int64_t second = (int64_t) ForthStack_remove(fmach->stack, length - 2);
    ForthStack_append(&fmach->stack, (void *) (second == first ? (int64_t) -1 : (int64_t) 0));
    return EXECUTE_OK;
}

