#include <stdbool.h>

#include "forthmachine.h"


int main(void)
{
    char program[] = "5 5 20 * + .s";
    struct forth_machine *fmach = forth_machine_init();
    bool load_success = forth_machine_load_program(fmach, program); 
    if(!load_success)
    {
        puts("machine failed to load the program");
    }
    forth_machine_run_program(fmach);
    forth_machine_deinit(fmach);
    return 0;
}
