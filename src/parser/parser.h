#ifndef PARSER_H
#define PARSER_H

size_t num_words(const char *str);

char **make_words_array(const char *str, size_t n);

void destroy_words_array(char **words, size_t n);

#endif
