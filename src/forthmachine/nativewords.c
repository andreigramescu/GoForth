#include <stdbool.h>
#include <inttypes.h>
#include "nativewords.h"
#include "forthmachine.h"
#include "forthstack.h"

static char *get_curr_word(struct forth_machine *fmach) {
  return fmach->program_words[fmach->program_counter];
}

enum error_code word_number(struct forth_machine *fmach) {
  char *endptr;
  int64_t val = strtoimax(get_curr_word(fmach), &endptr, 0);
  ForthStack_append(&(fmach->stack), val);
  return EXECUTE_OK;
 }

enum error_code word_dup(struct forth_machine *fmach) {
  ForthStack_append(&(fmach->stack), ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1));
  return EXECUTE_OK;
}

enum error_code word_drop(struct forth_machine *fmach) {
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  return EXECUTE_OK;
}

enum error_code word_plus(struct forth_machine *fmach) {
  int64_t top1 = ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  int64_t top2 = ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_append(&(fmach->stack), top1 + top2);
  return EXECUTE_OK;
}

enum error_code word_minus(struct forth_machine *fmach) {
  int64_t top1 = ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  int64_t top2 = ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_append(&(fmach->stack), top2 - top1);
  return EXECUTE_OK;
}


enum error_code word_star(struct forth_machine *fmach) {
  int64_t top1 = ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  int64_t top2 = ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_append(&(fmach->stack), top1 * top2);
  return EXECUTE_OK;
}

enum error_code word_slash(struct forth_machine *fmach) {
  int64_t top1 = ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  int64_t top2 = ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_append(&(fmach->stack), top2 / top1);
  return EXECUTE_OK;
}

enum error_code word_mod(struct forth_machine *fmach) {
  int64_t top1 = ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  int64_t top2 = ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_remove(fmach->stack, ForthStack_length(fmach->stack) - 1);
  ForthStack_append(&(fmach->stack), top2 % top1);
  return EXECUTE_OK;
}

enum error_code word_dot(struct forth_machine *fmach) {
  printf("%ld\n", ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1));
  return EXECUTE_OK;
}

enum error_code word_dot_s(struct forth_machine *fmach) {
  for (int i = 0; i < ForthStack_length(fmach->stack); i++) {
    printf("%ld ", ForthStack_get(fmach->stack, i));
  }
  puts("\n");
  return EXECUTE_OK;
}