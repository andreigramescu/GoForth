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

  char *prog1 = "andrei liam lauren";
  char *prog2 = "dup drop .s .";
  size_t n1 = num_words(prog1); 
  size_t n2 = num_words(prog2); 
  char **wds1 = make_words_array(prog1, n1);
  char **wds2 = make_words_array(prog2, n2);
  for (int i = 0; i < n1; i++) {
    puts(wds1[i]);
  }
  puts("---");
  for (int j = 0; j < n2; j++) {
    puts(wds2[j]);
  }
  size_t new_size;
  char **new_wds = append_words_array(wds1, n1, wds2, n2, &new_size);
  puts("---");
  for (int j = 0; j < new_size; j++) {
    puts(new_wds[j]);
  }
  printf("%lu", new_size); 

  destroy_words_array(new_wds, new_size);

  return 0;
}
