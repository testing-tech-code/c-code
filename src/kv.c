#include <kv.h>
#include <string.h>
#include <stdlib.h>

#define TOMBSTONE 0x1
#define _GNU_SOURCE



size_t hash(char *val,int capacity){
    size_t hash = 0x13371337deadbeef;

    while(*val){
        hash ^= *val;
        hash = hash << 8;
        hash += *val;

        val++;
    }
    return hash % capacity;
}

// fn kv_delete
// prams:
//  - db: a pointer to the db
//  - Kay: a pointer to the key value
//  - vaule: a porinter to the vaule itself
// return: the index of the deletion
// -1 if not found
int kv_delete(kv_t *db, char *key) {
    if (!db || !key ) return -1;

    size_t idx = hash(key, db->capacity);
     for (int i = 0; i < db->capacity -1; i++){
        size_t real_idx = (idx + i) % db->capacity;
        kv_entry_t *entry = &db->entries[real_idx];

        // is no key, therefor retrun nothing
         if (entry->key == NULL ) {
            return -1;
         }

         // find an entry and the keys match
         if (entry->key && entry->key != (void*)TOMBSTONE && !strcmp(entry->key, key)) {
            free(entry->key);
            free(entry->value);
            db->count--;
            entry->key = (void*)TOMBSTONE;
            entry->value = NULL;
            return real_idx;
         }
     }
     return -1;
}

// fn kv_get
// prams:
//  - db: a pointer to the db
//  - Kay: a pointer to the key value
//  - vaule: a porinter to the vaule itself
// return: the pointer to the key
// Null if not found
char *kv_get(kv_t *db, char *key) {
    if (!db || !key ) return NULL;

    size_t idx = hash(key, db->capacity);
     for (int i = 0; i < db->capacity -1; i++){
        size_t real_idx = (idx + i) % db->capacity;
        kv_entry_t *entry = &db->entries[real_idx];

        // is no key, therefor retrun nothing
         if (entry->key == NULL ) {
            return NULL;
         }

         // find an entry and the keys match
         if (entry->key && entry->key != (void*)TOMBSTONE && !strcmp(entry->key, key)) {
            return entry->value;
         }
     }

     return NULL;
}

// fn kv_free
// prams:
//  - db: a pointer to the db
// retruns: 0 on success, -1 on failure
int kv_free(kv_t *db){
    if (!db) return -1;


    for(size_t i = 0; i < db->capacity -1; i++){
        kv_entry_t *e = &db->entries[i];

        if(e->key && e->key != (void*)TOMBSTONE){
            free(e->key);
            free(e->value);
            e->key = NULL;
            e->value = NULL;
            db->count--;
        }
    }

    free(db->entries);
    free(db);

    return 0;

}

// fn kv_put
// prams:
//  - db: a pointer to the db
//  - Kay: a pointer to the key value
//  - vaule: a porinter to the vaule itself
// return: the index of the key, otherwiae, on
// error, retruns -1, on not found returns -2 
int kv_put(kv_t *db, char *key, char *value) {
    if (!db || !key || !value) return -1;

    size_t idx = hash(key, db->capacity);

    for (int i = 0; i < db->capacity -1; i++){
        
        size_t real_idx = (idx + i) % db->capacity;
        kv_entry_t *entry = &db->entries[real_idx];

        if (entry->key &&
            entry->key != (void*)TOMBSTONE &&
            !strcmp(entry->key,key)){
                char *newval = strdup(value);
                if (!newval) return -1;
                free(entry->value);
                entry->value = newval;
                return real_idx; 
        }

        if (!entry->key || entry->key == (void*)TOMBSTONE){
            char *newval = strdup(value);
            char *newkey = strdup(key);
            if(!newval || !newkey) {
                free(newkey);
                free(newval);
                return -1;
            }
                entry->value = newval;
                entry->key = newkey;
                db->count++;
                return real_idx;
        }
    }
    // the db is occupied
    return -2; 


}

kv_t *kv_init(size_t capacity){
    if (capacity == 0) return NULL;

    kv_t *table = malloc(sizeof(kv_t));
    if (table == NULL) {
        return NULL;
    }

    table->capacity = capacity;
    table->count = 0;
    table->entries = calloc(sizeof(kv_entry_t), capacity);
    if (table-> entries == NULL){
        return NULL;
    }

    return table;
}


