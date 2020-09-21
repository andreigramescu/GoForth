#include <stdio.h>
#include <stdbool.h>
#include "test.h"

void test_bool(bool toTest, const char *message) {
  printf("T: %s ----- %s\n", message, toTest ? "PASS" : "FAIL");
}