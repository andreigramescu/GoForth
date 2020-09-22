#include <stdbool.h>
#include <stdio.h>
#include "nativewords.h"
#include "parser.h"
#include "test.h"
#include "forthstack.h"
#include "forthmachine.h"

int main(void) {
  struct forth_machine random; 
  struct forth_machine *fmach = &random; 
  bool isInit = ForthMachine_init(fmach); 
  if (!isInit) {
    perror("Could not allocate forthmachine");
    return 1;
  }

  // Build forth machine
  ForthStack_append(&(fmach->stack), 2);
  ForthStack_append(&(fmach->stack), 8);
  fmach->program_words = make_words_array("90 34", 2);

  printf("%lu", ForthStack_length(fmach->stack));
  test_bool(ForthStack_length(fmach->stack) == 2, "stack size is initially 2");
  word_number(fmach);
  test_bool(ForthStack_length(fmach->stack) == 3, "stack size has grown by 1 after appending a number");
  test_bool(ForthStack_get(fmach->stack, 2) == 90, "appended 90 at the end of the stack after executing 90");
  test_bool(fmach->program_counter == 1, "program counter was incremented");

  // ForthMachine_deinit(fmach);
  return 0;
}