#include <stdbool.h>

#include "forthmachine.h"


char program1[] = ": dup 5 5 1 + .s ; dup dup dup";
char program2[] = ": word 5 5 1 + .s ; word dup word";


int main(void)
{
    struct forth_machine *fmach = forth_machine_init();
    bool load_success = forth_machine_load_program(fmach, program2); 
    if(!load_success)
    {
        puts("machine failed to load the program");
    }
    if(forth_machine_run_program(fmach) != EXECUTE_OK)
    {
        puts("failed execution");
    }
    forth_machine_deinit(fmach);
    return 0;
}
