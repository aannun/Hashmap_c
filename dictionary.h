#include <stdlib.h>
#include <string.h>

typedef struct dict_item
{
    size_t key_len;
    char* key;
    void* value;
    struct dict_item* next;
}dict_item_t;

typedef struct dict
{
    int hash_map_size;
    struct dict_item** hash_map;
}dict_t;

struct dict *dict_new(size_t hash_map_size);

void *dict_get(struct dict *dict, const char *key, size_t key_len);

void *dict_custom_hash_get(struct dict *dict, const char *key, size_t key_len, unsigned int (*custom_hash_ptr)(void *, size_t));

int dict_insert(struct dict *dict, const char *key, size_t key_len, void *value);

int dict_custom_hash_insert(struct dict *dict, const char *key, size_t key_len, void *value, unsigned int (*custom_hash_ptr)(void *, size_t));

int dict_length(struct dict *dict);

int dict_remove(struct dict *dict, const char *key, size_t key_len);

int dict_custom_hash_remove(struct dict *dict, const char *key, size_t key_len, unsigned int (*custom_hash_ptr)(void *, size_t));

void *dict_clear(struct dict *dict);