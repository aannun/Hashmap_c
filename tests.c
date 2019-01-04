#include "aiv_unit_test.h"
#include "dictionary.h"

#define Init_dict(len)  dict_t* dict = dict_new(len);\
    const char* test_key = "test_key";\
    int keylen = 8;\
    char* test_value = "test_value";

int test_hash(void *data, size_t keylen)
{
    return keylen;
}

int test_hash_fixed(void *data, size_t keylen)
{
    return 1;
}

TEST(dict_init)
{
    dict_t* dict = dict_new(100);
    ASSERT_THAT(dict->hash_map_size == 100);

    free(dict);
}

TEST(dict_length)
{
    Init_dict(100)

    ASSERT_THAT(dict_length(dict) == 0);

    dict_insert(dict, test_key, keylen, (void *)test_value);
    ASSERT_THAT(dict_length(dict) == 1);
    
    dict_insert(dict, "test_key2", 9, (void *)test_value);
    ASSERT_THAT(dict_length(dict) == 2);

    free(dict);
}

TEST(dict_insert)
{
    Init_dict(100)

    dict_insert(dict, test_key, keylen, (void *)test_value);

    ASSERT_THAT(dict_length(dict) == 1);

    free(dict);
}

TEST(dict_insert_multiple)
{
    Init_dict(100)

    int hash = test_hash_fixed(test_key, keylen) % dict->hash_map_size;
    dict_custom_hash_insert(dict, test_key, keylen, (void *)test_value, &test_hash_fixed);
    dict_custom_hash_insert(dict, "test_key2", 9, (void *)test_value, &test_hash_fixed);

    ASSERT_THAT(dict->hash_map[hash]->key == test_key);
    ASSERT_THAT(dict->hash_map[hash]->next->key == "test_key2");

    free(dict);
}

TEST(dict_custom_hash_insert)
{
    Init_dict(100)

    int hash = test_hash(test_key, keylen);
    dict_custom_hash_insert(dict, test_key, keylen, (void *)test_value, &test_hash);

    ASSERT_THAT(dict->hash_map[hash]->key == test_key);
    ASSERT_THAT(dict->hash_map[hash]->value == test_value);

    free(dict);
}

TEST(dict_insert_next)
{
    Init_dict(100)

    int hash = test_hash_fixed(test_key, keylen);
    dict_custom_hash_insert(dict, test_key, keylen, (void *)test_value, &test_hash_fixed);
    dict_custom_hash_insert(dict, "test_key_2", 10, (void *)"test_value_2", &test_hash_fixed);

    ASSERT_THAT(dict->hash_map[hash]->next->value == "test_value_2");

    free(dict);
}

TEST(dict_get)
{
    Init_dict(100)

    dict_insert(dict, test_key, keylen, (void *)test_value);
    void* value = dict_get(dict, test_key, keylen);

    ASSERT_THAT(value == test_value);   

    free(dict);
}

TEST(dict_custom_hash_get)
{
    Init_dict(100)

    dict_custom_hash_insert(dict, test_key, keylen, (void *)test_value, &test_hash);
    void* value = dict_custom_hash_get(dict, test_key, keylen, &test_hash);

    ASSERT_THAT(value == test_value);   

    free(dict);
}

TEST(dict_replace)
{
    Init_dict(100)

    int hash = test_hash(test_key, keylen) % dict->hash_map_size;
    dict_custom_hash_insert(dict, test_key, keylen, (void *)test_value, &test_hash);

    const char *test_value_2 = "replace_value";
    dict_custom_hash_insert(dict, test_key, keylen, (void *)test_value_2, &test_hash);

    ASSERT_THAT((char *)dict->hash_map[hash]->value == test_value_2);

    free(dict);
}

TEST(dict_remove)
{
    Init_dict(100)

    dict_insert(dict, test_key, keylen, (void *)test_value);
    dict_remove(dict, test_key, keylen);

    ASSERT_THAT(dict_length(dict) == 0);

    free(dict);
}

TEST(dict_custom_hash_remove)
{
    Init_dict(100)

    int hash = test_hash(test_key, keylen) % dict->hash_map_size;
    dict_custom_hash_insert(dict, test_key, keylen, (void *)test_value, &test_hash);
    dict_custom_hash_remove(dict, test_key, keylen, &test_hash);

    ASSERT_THAT(dict->hash_map[hash] == NULL);

    free(dict);
}

TEST(dict_remove_multiple_insert)
{
    Init_dict(100)

    int hash = test_hash_fixed(test_key, keylen) % dict->hash_map_size;
    dict_custom_hash_insert(dict, test_key, keylen, (void *)test_value, &test_hash_fixed);
    dict_custom_hash_insert(dict, "test_key2", 9, (void *)test_value, &test_hash_fixed);
    dict_custom_hash_insert(dict, "test_key3", 9, (void *)test_value, &test_hash_fixed);

    dict_custom_hash_remove(dict, "test_key2", 9, &test_hash_fixed);

    ASSERT_THAT(dict->hash_map[hash]->next->key == "test_key3");

    free(dict);
}

TEST(dict_length_after_remove)
{
    Init_dict(100)

    dict_insert(dict, test_key, keylen, (void *)test_value);
    dict_insert(dict, "test_key2", 9, (void *)test_value);

    dict_remove(dict, test_key, keylen);

    ASSERT_THAT(dict_length(dict) == 1);

    free(dict);
}

TEST(dict_clear)
{
    Init_dict(100)

    int hash = test_hash(test_key, keylen) % dict->hash_map_size;
    dict_custom_hash_insert(dict, test_key, keylen, (void *)test_value, &test_hash);
    dict_custom_hash_insert(dict, "test_key2", 9, (void *)test_value, &test_hash);

    dict_clear(dict);

    ASSERT_THAT(dict_length(dict) == 0);
    ASSERT_THAT(dict->hash_map[hash] == NULL);

    free(dict);
}

int main(int argc, char **argv)
{
	RUN_TEST(dict_init);
	RUN_TEST(dict_length);
	RUN_TEST(dict_insert);
	RUN_TEST(dict_insert_multiple);
	RUN_TEST(dict_custom_hash_insert);
	RUN_TEST(dict_insert_next);
	RUN_TEST(dict_get);
	RUN_TEST(dict_custom_hash_get);
	RUN_TEST(dict_replace);
	RUN_TEST(dict_remove);
	RUN_TEST(dict_custom_hash_remove);
	RUN_TEST(dict_remove_multiple_insert);
	RUN_TEST(dict_length_after_remove);
	RUN_TEST(dict_clear);

	PRINT_TEST_RESULTS();
	return 0;
}