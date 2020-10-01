#ifndef PARSER_H
#define PARSER_H

size_t num_words(const char *str);

char **make_words_array(const char *str, size_t n);

char **append_words_array(char **arr1, size_t n, char **arr2, size_t m, size_t *new_size);

void destroy_words_array(char **words, size_t n);

#endif
