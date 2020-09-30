#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "forthmachine.h"

#define MAX_CHARS 10000

char program1[] = ": dup 5 5 1 + .s ; dup dup dup";
char program2[] = ": word 5 5 1 + .s ; word dup word";


int main(void)
{
    puts("---GoForth---");
    puts("Entering REPL mode...");

    struct forth_machine *fmach = forth_machine_init();

    // for now
    char *input = malloc(MAX_CHARS * sizeof(char));
    if (!input) {
      puts("Something went wrong");
      forth_machine_deinit(fmach);
      return 1;
    }
    while (strcmp(input, "quit")) {
      int i = 0;
      char c;
      while ((c = getchar()) != '\n') {
        assert(i < MAX_CHARS);
        input[i++] = c; 
      }
      assert(i < MAX_CHARS);
      input[i] = 0;

      bool load_success = forth_machine_load_program(fmach, input);
      if(!load_success) {
          puts("machine failed to load the program");
      }
      if (forth_machine_run_program(fmach) != EXECUTE_OK) {
          puts("something went wrong while trying to run program");
      }    
    }


   /* bool load_success = forth_machine_load_program(fmach, program2); */ 
    /* if(!load_success) */
    /* { */
    /*     puts("machine failed to load the program"); */
    /* } */
    /* if(forth_machine_run_program(fmach) != EXECUTE_OK) */
    /* { */
    /*     puts("failed execution"); */
    /* } */
    /* forth_machine_deinit(fmach); */
    return 0;
}
