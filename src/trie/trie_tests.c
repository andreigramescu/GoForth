#include <stdio.h>
#include "test_utils.h"

#include "trie.h"
int main(void)
{
    Trie *trie = Trie_create();  
    Trie_add(trie, "hello", (void *) 0);
    Trie_add(trie, "hello_world",(void *) 1);
    Trie_add(trie, "zello", (void *) 2);
    Trie_add(trie, "h3llo", (void *) 3);
    Trie_add(trie, "he=lo", (void *) 4);
    Trie_add(trie, "q", (void *) 5);
    Trie_add(trie, "w", (void *) 6);
    Trie_add(trie, "a", (void *) 7);
    Trie_add(trie, "b", (void *) 8);
    Trie_add(trie, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890~!@#$%^&*()_+,./;'[]\\<>?:\"{}|", (void *) 9);
    bool intrie;
    void *val; 
    intrie = Trie_get(trie, "hello", &val);
    testbool(val == (void *) 0 && intrie == true, "hello is in the trie");
    intrie = Trie_get(trie, "hello_world", &val);
    testbool(val == (void *) 1 && intrie == true, "hello_world is in the trie");
    intrie = Trie_get(trie, "zello", &val);
    testbool(val == (void *) 2 && intrie == true, "zello is in the trie");
    intrie = Trie_get(trie, "h3llo", &val);
    testbool(val == (void *) 3 && intrie == true, "h3llo is in the trie");
    intrie = Trie_get(trie, "he=lo", &val);
    testbool(val == (void *) 4 && intrie == true, "he=lo is in the trie");
    intrie = Trie_get(trie, "q", &val);
    testbool(val == (void *) 5 && intrie == true, "q is in the trie");
    intrie = Trie_get(trie, "w", &val);
    testbool(val == (void *) 6 && intrie == true, "w is in the trie");
    intrie = Trie_get(trie, "a", &val);
    testbool(val == (void *) 7 && intrie == true, "a is in the trie");
    intrie = Trie_get(trie, "b", &val);
    testbool(val == (void *) 8 && intrie == true, "b is in the trie");
    intrie = Trie_get(trie, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890~!@#$%^&*()_+,./;'[]\\<>?:\"{}|", &val);
    testbool(val == (void *) 9 && intrie == true, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890~!@#$%^&*()_+,./;'[]\\<>?:\"{}| is in the trie");
    
    Trie_destroy(trie);
    return 0;
}
