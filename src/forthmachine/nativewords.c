#include <stdbool.h>
#include <inttypes.h>
#include "nativewords.h"
#include "forthmachine.h"
#include "forthstack.h"

static char *get_curr_word(struct forth_machine *fmach) {
  return fmach->program_words[fmach->n_program_words];
}

enum error_code word_number(struct forth_machine *fmach) {
  char *endptr;
  int64_t val = strtoimax(get_curr_word(fmach), &endptr, 0);
  ForthStack_append(&(fmach->stack), val);
  (fmach->program_counter)++;
  return EXECUTE_OK;
 }
