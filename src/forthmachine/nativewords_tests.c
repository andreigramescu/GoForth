#include <stdbool.h>
#include <stdio.h>
#include "nativewords.h"
#include "parser.h"
#include "test.h"
#include "forthstack.h"
#include "forthmachine.h"

int main(void) {
  struct forth_machine *fmach = forth_machine_init(); 
  if (fmach == NULL) {
    perror("Could not allocate forthmachine");
    return 1;
  }

  // Build forth machine
  ForthStack_append(&(fmach->stack), 2);
  ForthStack_append(&(fmach->stack), 8);
  fmach->program_words = make_words_array("90 2 96 2", 4);

  test_bool(ForthStack_length(fmach->stack) == 2, "stack size is initially 2");
  word_number(fmach);
  test_bool(ForthStack_length(fmach->stack) == 3, "stack size has grown by 1 after appending a number");
  test_bool(ForthStack_get(fmach->stack, 2) == 90, "appended 90 at the end of the stack after executing 90");
  word_dot_s(fmach);

  puts("\nRunning duplicate word...");
  word_dup(fmach);
  test_bool(ForthStack_length(fmach->stack) == 4, "stack size has grown by 1 after duplicating top item");
  test_bool(ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1) == 90, "Correctly duplicated 90");

  puts("\nRunning drop word...");
  word_drop(fmach);
  test_bool(ForthStack_length(fmach->stack) == 3, "stack length has droped by 1 after dropping top item");
  test_bool(ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1) == 90, "correctlu dropped 90");

  puts("\nRunning plus command..."); 
  word_plus(fmach);
  test_bool(ForthStack_length(fmach->stack) == 2, "stack length has dropped by 1 after adding top items");
  test_bool(ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1) == 98, "Correctly added the top 2 items from the stack");
  word_dot(fmach);

  puts("\nRunning minus command..."); 
  word_minus(fmach);
  test_bool(ForthStack_length(fmach->stack) == 1, "stack length has dropped by 1 after subtracting top items");
  test_bool(ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1) == -96, "Correctly subtracted the top 2 items from the stack");
  word_dot(fmach);


  (fmach->program_counter)++;
  word_number(fmach);
  puts("\nRunning star command..."); 
  word_star(fmach);
  test_bool(ForthStack_length(fmach->stack) == 1, "stack length has dropped by 1 after multiplying top items");
  test_bool(ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1) == -192, "Correctly multiplied the top 2 items from the stack");



  (fmach->program_counter)++;
  word_number(fmach);
  puts("\nRunning slash command..."); 
  word_slash(fmach);
  test_bool(ForthStack_length(fmach->stack) == 1, "stack length has dropped by 1 after multiplying top items");
  test_bool(ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1) == -2, "Correctly divided the top 2 items from the stack");


  (fmach->program_counter)++;
  word_number(fmach);
  puts("\nRunning mod command..."); 
  word_mod(fmach);
  test_bool(ForthStack_length(fmach->stack) == 1, "stack length has dropped by 1 after modding top items");
  test_bool(ForthStack_get(fmach->stack, ForthStack_length(fmach->stack) - 1) == 0, "Correctly modded the top 2 items from the stack");
  word_dot(fmach);
  // ForthMachine_deinit(fmach);
  return 0;
}
