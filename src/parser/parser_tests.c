#include <malloc.h>
#include <string.h>
#include "test.h"
#include "parser.h"

int main(void) {
  char *input_string = "andrei liam 1 2 3 ; , dsaf,ds,,,,,,,, kjfdkjfkdjjjjjjjj _______+========    \t\n  ";
  char *input_string2 = "dup dup drop 1 2 2435 .s .";
  char **words = make_words_array(input_string, 10);
  char **words2 = make_words_array(input_string2, 8);
  test_bool(num_words(input_string) == 10, "first input string has 10 words");
  test_bool(num_words(input_string2) == 8, "second input string has 8 words");
  test_bool(!strcmp(words[0], "andrei"), "first word of first input string is andrei");
  test_bool(!strcmp(words2[2], "drop"), "third word of second input string is drop");
  test_bool(!strcmp(words[4], "3"), "fifth words of first input string is 3");
  test_bool(!strcmp(words[4], "3"), "fifth words of second input string is 2435");
  destroy_words_array(words, 10);
  destroy_words_array(words2, 8);

  return 0;
}