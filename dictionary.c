#include "dictionary.h"

static int hash_pomettini(const char *data, size_t len)
{
    int value = 0;
    size_t i;
    for (i = 0; i < len; i++)
    {
        value += data[i];
    }

    return value;
}

static int hash_flashdance(const char *data, size_t len)
{
    return len;
}

struct aiv_dict *aiv_dict_new(size_t hash_map_size)
{
    struct aiv_dict *new_dict = malloc(sizeof(aiv_dict_t));
    if (!new_dict)
        return NULL;
    // useless optimization: there is a mul opcode
    //memset(new_dict->hash_map, 0, sizeof(aiv_dict_item_t *) * hash_map_size);
    // good optimization if size is known "a priori"
    //memset(new_dict, 0, sizeof(struct aiv_dict));
    size_t hash_map_memory_size = sizeof(struct aiv_dict_item *) * hash_map_size;
    new_dict->hash_map = malloc(hash_map_memory_size);
    if (!new_dict->hash_map)
    {
        free(new_dict);
        return NULL;
    }
    memset(new_dict->hash_map, 0, hash_map_memory_size);
    new_dict->hash_map_size = hash_map_size;
    return new_dict;
}

void *aiv_dict_get(struct aiv_dict *dict, const char *key, size_t key_len)
{
    int hash = hash_flashdance(key, key_len) % dict->hash_map_size;
    struct aiv_dict_item *current = dict->hash_map[hash];
    while (current)
    {
        if (current->key_len == key_len && !memcmp(current->key, key, key_len))
        {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

int aiv_dict_insert(struct aiv_dict *dict, const char *key, size_t key_len, void *value)
{
    int hash = hash_flashdance(key, key_len) % dict->hash_map_size;
    struct aiv_dict_item *current = dict->hash_map[hash];
    struct aiv_dict_item *last_current = current;

    while (current)
    {
        if (current->key_len == key_len && !memcmp(current->key, key, key_len))
        {
            current->value = value;
            return 0;
        }
        last_current = current;
        current = current->next;
    }

    struct aiv_dict_item *new_dict_item = malloc(sizeof(aiv_dict_item_t));
    if (new_dict_item == NULL)
    {
        return -1;
    }
    new_dict_item->key = key;
    new_dict_item->key_len = key_len;
    new_dict_item->next = NULL;
    new_dict_item->value = value;

    if (last_current != NULL)
        last_current->next = new_dict_item;
    else
    {
        dict->hash_map[hash] = new_dict_item;
    }

    return 0;
}