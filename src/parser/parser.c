#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "parser.h"

#define WHITESPACE " \t\n"

static char *strdup(const char *str);

size_t num_words(const char *str) {
  // str will get destroyed by strtok so saving a copy
  char *copy = strdup(str);
  char *token = strtok(copy, WHITESPACE);
  size_t res = 0;

  while (token != NULL) {
    res++;
    token = strtok(NULL, WHITESPACE);
  }

  free(copy);
  return res;
}

char **make_words_array(const char *str, size_t n) {
  // Caller is responsible for freeing the input String
  assert(n != 0);

  char *copy = strdup(str);
  if (!copy) {
    return NULL;
  }
  char **res = calloc(sizeof(char *), n);
  if (!res) {
    free(copy);
    return NULL;
  }
  char *token = strtok(copy, WHITESPACE);
  size_t i = 0;

  while (token != NULL) {
    assert(i < n);
    res[i++] = strdup(token);
    if (!res[i-1]) {
      for (size_t j = 0; j < i - 1; j++) {
        free(res[j]);
      }
      free(res);
      free(copy);
      return NULL;
    }
    token = strtok(NULL, WHITESPACE);
  }

  free(copy);
  return res;
}

char **append_words_array(char **arr1, size_t n, char **arr2, size_t m, size_t *new_size) {
  *new_size = n + m;
  char **res = malloc(*new_size * sizeof(char *));
  for (int i = 0; i < n; i++) {
    res[i] = arr1[i];
  }
  for (int j = 0; j < m; j++) {
    res[n + j] = arr2[j];
  }

  free(arr1);
  free(arr2);
  return res;
}

void destroy_words_array(char **words, size_t n) {
  assert(words);
  for (int i = 0; i < n; i++) {
    free(words[i]);
  }
  free(words);
}

static char *strdup(const char *str) {
  char *res = malloc((strlen(str) + 1) * sizeof(char));
  if (!res) {
    return NULL;
  }
  strcpy(res, str);
  return res;
}
