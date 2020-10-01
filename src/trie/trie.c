#include "trie.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

/* 128 bit bitfield to store the majority of ascii codes. 
* bit 0 is the value bit. It is set when the word is in the trie*/
struct trie_node
{
    uint64_t bitfield[2];
    void *value;
    struct trie_node **children;
};

static unsigned char get_bit_pos(char symbol);

static unsigned char get_nchildren(uint64_t bitfield1, uint64_t bitfield2);
    
static unsigned char nbits_set(uint64_t bitfield);

static bool bit_is_set(uint64_t bitfield1, uint64_t bitfield2, unsigned char bit_pos);

static unsigned char get_child_index(
        uint64_t bitfield1, uint64_t bitfield2, unsigned char bit_pos
        );

Trie *Trie_create(void)
{
    struct trie_node *trie = (struct trie_node *) malloc(sizeof(struct trie_node)); 
    if(trie == NULL)
    {
        return NULL;
    }
    trie->bitfield[0] = trie->bitfield[1] = 0;
    trie->value = NULL;
    trie->children = NULL;
    return (Trie *) trie;
}

void Trie_destroy(Trie *trie, Trie_destruction_function func)
{
    assert(trie != NULL); 
    struct trie_node *curr = trie;
    unsigned char nchildren = get_nchildren(curr->bitfield[0], curr->bitfield[1]);
    for(size_t i = 0; i < (uint64_t)nchildren; i++ )
    {
        Trie_destroy(curr->children[i], func);
    }
    func(curr->value);
    free(curr->children);
    free(curr);
}

bool Trie_add(Trie *trie, const char *word, void *value)
{
    assert(trie != NULL && word != NULL); 
    struct trie_node *curr = (struct trie_node *) trie;
    for(const char *c = word; *c != '\0'; c++)
    {
        char bit_pos = get_bit_pos(*c);
        size_t index = get_child_index(curr->bitfield[0], curr->bitfield[1], bit_pos);
        if(bit_is_set(curr->bitfield[0], curr->bitfield[1], bit_pos)) 
        {
            curr = curr->children[index];           
        } else
        {
            struct trie_node *child = Trie_create();
            if(child == NULL)
            {
                return false;
            }
            unsigned char nchildren = get_nchildren(curr->bitfield[0], 
                                                    curr->bitfield[1]);
            struct trie_node **children = (struct trie_node **) realloc(curr->children, 
                                                              (nchildren + 1) * 
                                                              sizeof(struct trie_node *)
                            );
            if(children == NULL)
            {
                free(child);
                return false;
            } 
            curr->children = children;
            /* rearange children */
            for(size_t i = nchildren; i > index; i--)
            {
                curr->children[i] = curr->children[i-1];
            }
            curr->children[index] = child;    
            if(bit_pos >= 64)
            {
                curr->bitfield[1] |= (((uint64_t)1) << (bit_pos - 64)); 
            } else
            {
                curr->bitfield[0] |= (((uint64_t)1) << bit_pos); 
            }
            curr = curr->children[index];           
        }
    }
    if(curr->bitfield[0] & 1)
    {
        return false;
    }
    curr->value = value;
    curr->bitfield[0] |= 1;
    return true;
}

bool Trie_get(const Trie *trie, const char *key, void **value)
{
    assert(trie != NULL && value != NULL && key != NULL); 
    const struct trie_node *curr = (const struct trie_node *) trie;
    for(const char *c = key; *c != '\0'; c++)
    {
        char bit_pos = get_bit_pos(*c);
        size_t index = get_child_index(curr->bitfield[0], curr->bitfield[1], bit_pos);           
        if(bit_is_set(curr->bitfield[0], curr->bitfield[1], bit_pos)) 
        {
            curr = curr->children[index];           
        } else
        {
            return false;
        }
    }
    *value = curr->value;
    return curr->bitfield[0] & 1;
}

static unsigned char get_bit_pos(char symbol)
{
    assert(symbol >= '!' && symbol <= '~');
    return symbol - '!' + 1;
}


static unsigned char nbits_set(uint64_t bitfield)
{
    unsigned char cnt = 0;
    while(bitfield != 0)
    {
        cnt += bitfield & 1;
        bitfield >>= 1;
    }
    return cnt;
}

static unsigned char get_nchildren(uint64_t bitfield1, uint64_t bitfield2)
{
    return nbits_set(bitfield1) + nbits_set(bitfield2) - (bitfield1 & 1);
}

static bool bit_is_set(uint64_t bitfield1, uint64_t bitfield2, unsigned char bit_pos)
{

    if(bit_pos >= 64)
    {
        return (bitfield2 >> (bit_pos - 64)) & 1;
    } else 
    {
        return (bitfield1 >> bit_pos) & 1; 
    } 
}

static unsigned char get_child_index(
        uint64_t bitfield1, uint64_t bitfield2, unsigned char bit_pos
        )
{
    unsigned char index = 0;
    assert(bit_pos >= 1 && bit_pos <= 128);
    if(bit_pos > 64)
    {
        uint64_t bitmask = ~((uint64_t)0) >> (128 - bit_pos);
        index = nbits_set(bitfield1) + nbits_set(bitfield2 & bitmask);
    } else
    {
        uint64_t bitmask = ~((uint64_t)0) >> (64 - bit_pos);
        index = nbits_set(bitfield1 & bitmask);
    }
    return index - (bitfield1 & 1);
}


#if !NDEBUG

#include <stdio.h>

void Trie_dump(const Trie *trie, int depth)
{
    assert(trie != NULL); 
    const struct trie_node *curr = trie;
    char symbol = '!';
    size_t index = 0;
    uint64_t bitfield1 = curr->bitfield[0];
    uint64_t bitfield2 = curr->bitfield[1];
    bitfield1 >>= 1;
    while(bitfield1 != 0)
    {
        if(bitfield1 & 1)
        {
            putc('\n', stdout);
            for(int i = 0; i < depth; i++) putc('-', stdout);
            putc(symbol, stdout);
            Trie_dump(curr->children[index], depth + 1);
            index++;
        } 
        symbol++;
        bitfield1 >>= 1;
    }   
    symbol = 63 + '!';
    while(bitfield2 != 0)
    {
        if(bitfield2 & 1)
        {
            putc('\n', stdout);
            for(int i = 0; i < depth; i++) putc('-', stdout);
            putc(symbol, stdout);
            Trie_dump(curr->children[index], depth + 1);
            index++;
        } 
        symbol++;
        bitfield2 >>= 1;
    }
    putc(' ', stdout);
}

#endif

