#include <stdio.h>
#include <stdbool.h>
#include "test.h"

void test_bool(bool toTest, const char *message) {
  printf("T: %s ----- ", message);
  if (toTest) {
    printf("\033[1;32m"); 
    printf("PASS\n");
    printf("\033[0m"); 
    return;
  }
  printf("\033[1;31m");
  printf("FAIL\n");
  printf("\033[0m"); 
}