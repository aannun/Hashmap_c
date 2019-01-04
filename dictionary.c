#include "dictionary.h"

//HASH
static unsigned int dict_get_hash(void *data, size_t len)
{
    unsigned int hash = 5381;
    unsigned int i = 0;
    char* ptr = data;

    for(i = 0; i < len; i++)
    {   
        hash += hash << 5;
        hash += *ptr;
        ptr++;
    }   

    return hash;
}

//INIT
struct dict *dict_new(size_t hash_map_size)
{
    size_t hash_map_memory_size = sizeof(struct dict_item *) * hash_map_size;
    struct dict *new_dict = malloc(sizeof(dict_t) + hash_map_memory_size);

    if (!new_dict)
        return NULL;

    new_dict->hash_map = (unsigned long long)new_dict + sizeof(dict_t);
    memset(new_dict->hash_map, 0, hash_map_memory_size);
    new_dict->hash_map_size = hash_map_size;

    return new_dict;
}

//LENGTH
int dict_length(struct dict *dict)
{
    int i = 0;
    int length = 0;
    struct dict_item *current = NULL;

    for(i = 0; i < dict->hash_map_size; i++)
    {
        current = dict->hash_map[i];
        while (current)
        {
            current = current->next;
            length++;
        }
    }

    return length;
}

//GET
void *dict_get(struct dict *dict, const char *key, size_t key_len)
{
    return dict_custom_hash_get(dict, key, key_len, &dict_get_hash);
}

void *dict_custom_hash_get(struct dict *dict, const char *key, size_t key_len, unsigned int (*custom_hash_ptr)(void *, size_t))
{
    unsigned int hash = (*custom_hash_ptr)((void *)key, key_len) % dict->hash_map_size;
    struct dict_item *current = dict->hash_map[hash];
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

//INSERT
int dict_insert(struct dict *dict, const char *key, size_t key_len, void *value)
{
    return dict_custom_hash_insert(dict, key, key_len, value, &dict_get_hash);
}

int dict_custom_hash_insert(struct dict *dict, const char *key, size_t key_len, void *value, unsigned int (*custom_hash_ptr)(void *, size_t))
{
    unsigned int hash = (*custom_hash_ptr)((void *)key, key_len) % dict->hash_map_size;
    struct dict_item *current = dict->hash_map[hash];
    struct dict_item *last_current = current;

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

    struct dict_item *new_dict_item = malloc(sizeof(dict_item_t));
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

//REMOVE
int dict_remove(struct dict *dict, const char *key, size_t key_len)
{
    return dict_custom_hash_remove(dict, key, key_len, &dict_get_hash);
}

int dict_custom_hash_remove(struct dict *dict, const char *key, size_t key_len, unsigned int (*custom_hash_ptr)(void *, size_t))
{
    unsigned int hash = (*custom_hash_ptr)((void *)key, key_len) % dict->hash_map_size;
    struct dict_item *current = dict->hash_map[hash];
    struct dict_item *last_current = NULL;

    while (current)
    {
        if (current->key_len == key_len && !memcmp(current->key, key, key_len))
        {
            if(last_current != NULL)
            {
                if(current->next != NULL)
                    last_current->next = current->next;
                else
                    last_current->next = NULL;
            }
            else
                dict->hash_map[hash] = NULL;

            free(current);
            return 0;
        }
        last_current = current;
        current = current->next;
    }
    return -1;
}

//CLEAR
void *dict_clear(struct dict *dict)
{
    int i = 0;
    struct dict_item *current = NULL;
    struct dict_item *last_current = NULL;

    for(i = dict->hash_map_size - 1; i >= 0; i--)
    {
        current = dict->hash_map[i];
        while (current)
        {
            last_current = current;
            current = current->next;

            free(last_current);
        }
        dict->hash_map[i] = NULL;
    }
}