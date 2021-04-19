#pragma once
#include "tree.h"
#include <stdio.h>
#include <malloc.h>

using namespace std;

//переписать под список для юзеров

typedef struct HashTab {
    struct {
        vtype_tree_t key;
        vtype_tree_t value;
    } type;
    size_t size;
    Tree** table;
} HashTab;

extern void free_hashtab(HashTab* hashtab);

extern value_tree_t get_hashtab(HashTab* hashtab, void* key);
extern void set_hashtab(HashTab* hashtab, void* key, void* value);
extern void del_hashtab(HashTab* hashtab, void* key);
extern bool in_hashtab(HashTab* hashtab, void* key);

extern void print_hashtab(HashTab* hashtab);

static uint32_t _strhash(const char* s, size_t size);



static HashTab* new_hashtab(size_t size, vtype_tree_t key, vtype_tree_t value) {
    switch (key) {
    case DECIMAL_ELEM: case STRING_ELEM:
        break;
    default:
        fprintf(stderr, "%s\n", "key type not supported");
        return NULL;
    }
    switch (value) {
    case DECIMAL_ELEM: case REAL_ELEM: case STRING_ELEM:
        break;
    default:
        fprintf(stderr, "%s\n", "value type not supported");
        return NULL;
    }
    HashTab* hashtab = (HashTab*)malloc(sizeof(HashTab));
    hashtab->table = (Tree**)malloc(size * sizeof(Tree));
    for (size_t i = 0; i < size; ++i) {
        hashtab->table[i] = new_tree(key, value);
    }
    hashtab->size = size;
    hashtab->type.key = key;
    hashtab->type.value = value;
    return hashtab;
}

extern void del_hashtab(HashTab* hashtab, void* key) {
    uint32_t hash;
    switch (hashtab->type.key) {
    case DECIMAL_ELEM:
        hash = (uint64_t)key % hashtab->size;
        break;
    case STRING_ELEM:
        hash = _strhash((const char*)key, hashtab->size);
        break;
    }
    del_tree(hashtab->table[hash], key);
}

extern bool in_hashtab(HashTab* hashtab, void* key) {
    uint32_t hash;
    bool result;
    switch (hashtab->type.key) {
    case DECIMAL_ELEM:
        hash = (uint64_t)key % hashtab->size;
        break;
    case STRING_ELEM:
        hash = _strhash((const char*)key, hashtab->size);
        break;
    }
    result = in_tree(hashtab->table[hash], key);
    return result;
}

extern value_tree_t get_hashtab(HashTab* hashtab, void* key) {
    uint32_t hash;
    value_tree_t result;
    switch (hashtab->type.key) {
    case DECIMAL_ELEM:
        hash = (uint64_t)key % hashtab->size;
        break;
    case STRING_ELEM:
        hash = _strhash((const char*)key, hashtab->size);
        break;
    }
    result = get_tree(hashtab->table[hash], key);
    return result;
}

extern void set_hashtab(HashTab* hashtab, void* key, void* value) {
    uint32_t hash;
    switch (hashtab->type.key) {
    case DECIMAL_ELEM:
        hash = (uint64_t)key % hashtab->size;
        break;
    case STRING_ELEM:
        hash = _strhash((const char*)key, hashtab->size);
        break;
    }
    set_tree(hashtab->table[hash], key, value);
}

extern void free_hashtab(HashTab* hashtab) {
    for (size_t i = 0; i < hashtab->size; ++i) {
        free_tree(hashtab->table[i]);
    }
    free(hashtab->table);
    free(hashtab);
}

extern void print_hashtab(HashTab* hashtab) {
    printf("{\n");
    int hash;
    for (size_t i = 0; i < hashtab->size; ++i) {
        if (hashtab->table[i]->node == NULL) {
            continue;
        }
         switch(hashtab->type.key) {
          case DECIMAL_ELEM:
              hash = hashtab->table[i]->node->data.key.decimal % hashtab->size;
          break;
          case STRING_ELEM:
              hash = _strhash(hashtab->table[i]->node->data.key.string, hashtab->size);
              printf("№%d - hash:%d\n", i, hash);
          break;
         }
        printf("\t%d => ", i);
        print_tree_as_list(hashtab->table[i]);
    }
    printf("}\n");
}

static uint32_t _strhash(const char* s, size_t size) {
    uint32_t hashval;
    for (hashval = 0; *s != '\0'; ++s) {
        hashval = *s + 31 * hashval;
    }
    return hashval % size;
}
//поменять хеш-функцию, усложнить

