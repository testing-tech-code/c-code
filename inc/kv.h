#ifndef KV_H
#define KV_H

#include <stdlib.h>

typedef struct {
    char *key;
    char *value; 
} KV_entry_t;

typedef struct {
    size_t capacity;
    size_t count;
    KV_entry_t *entries;
} kv_t;

kv_t *kv_init(size_t capacity);

#endif