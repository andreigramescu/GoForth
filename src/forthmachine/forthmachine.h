#ifndef FORTHMACHINE_H
#define FORTHMACHINE_H 

#include <stdbool.h>

#include "common.h"

struct forth_machine *forth_machine_init();

void forth_machine_deinit(struct forth_machine *fmach);

bool forth_machine_load_program(struct forth_machine *fmach, const char *program);

enum error_code forth_machine_run_program(struct forth_machine *fmach);

#endif /* FORTHMACHINE_H */
