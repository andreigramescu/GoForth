#ifndef TRIE_H
#define TRIE_H 

#include <stdint.h>
#include <stdbool.h>


typedef void Trie;

Trie *Trie_create(void);

void Trie_destroy(Trie *trie);

bool Trie_add(Trie *trie, const char *word, void *value);

bool Trie_get(const Trie *trie, const char *key, void **value);


#if !NDEBUG

void Trie_dump(const Trie *trie, int depth);

#endif

#endif
