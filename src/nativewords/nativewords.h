#ifndef NATIVEWORDS_H
#define NATIVEWORDS_H

#include "forthmachine.h"

enum error_code {
  EXECUTE_OK,
};

typedef enum error_code (*word_execution)(struct forth_machine *fmach);

#define WORD_FUNCTION_DECLARATION(name) enum error_code word_##name(struct forth_machine *fmach)

WORD_FUNCTION_DECLARATION(number);
WORD_FUNCTION_DECLARATION(dup);
WORD_FUNCTION_DECLARATION(drop);
WORD_FUNCTION_DECLARATION(plus);
WORD_FUNCTION_DECLARATION(minus);
WORD_FUNCTION_DECLARATION(star);
WORD_FUNCTION_DECLARATION(slash);
WORD_FUNCTION_DECLARATION(mod);
WORD_FUNCTION_DECLARATION(dot);
WORD_FUNCTION_DECLARATION(dots);

#endif