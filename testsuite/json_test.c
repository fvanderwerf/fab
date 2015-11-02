
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "json_test.h"

#include <json.h>

#include <string.h>
#include <stdlib.h>

struct json_string_token {
    struct fab_json_token token;
    struct json_string_token *next;
};

struct json_string_token *json_string = NULL;

static void json_string_clear()
{
    struct json_string_token *token = json_string;
    while (token != NULL) {
        struct json_string_token *previous = token;
        token = token->next;
        free(previous);
    }

    json_string = NULL;
}

static void json_string_append(const struct fab_json_token *token, void *data)
{
    if (json_string == NULL) {
        json_string = malloc(sizeof(struct fab_json_token));
        json_string->token = *token;
        json_string->next = NULL;
    } else {
        struct json_string_token *last = json_string;

        while (last->next != NULL)
            last = last->next;

        last->next = malloc(sizeof(struct fab_json_token));
        last->next->token = *token;
        last->next->next = NULL;
    }
}

void create()
{
    fab_json_t json = NULL;

    json = fab_json_create(json_string_append, NULL);

    ck_assert(json != NULL);
    ck_assert_ptr_eq(NULL, json_string);

    fab_json_destroy(json);
}

void test_null()
{
    fab_json_t json = NULL;
    json_string_clear();

    json = fab_json_create(json_string_append, NULL);

    ck_assert(json != NULL);

    fab_json_process(json, 'n');
    fab_json_process(json, 'u');
    fab_json_process(json, 'l');
    fab_json_process(json, 'l');

    ck_assert(json_string != NULL);
    ck_assert(FAB_JSON_NULL == json_string->token.type);
    ck_assert_ptr_eq(NULL, json_string->next);

    fab_json_destroy(json);
}

void test_false()
{
    int i;
    fab_json_t json = NULL;
    json_string_clear();

    json = fab_json_create(json_string_append, NULL);

    ck_assert(json != NULL);

    for (i = 0; i < 5; i++)
        fab_json_process(json, "false"[i]);

    ck_assert(json_string != NULL);
    ck_assert(FAB_JSON_FALSE == json_string->token.type);
    ck_assert_ptr_eq(NULL, json_string->next);

    fab_json_destroy(json);
}


void test_true()
{
    int i;
    fab_json_t json = NULL;
    json_string_clear();

    json = fab_json_create(json_string_append, NULL);

    ck_assert(json != NULL);

    for (i = 0; i < 4; i++)
        fab_json_process(json, "true"[i]);

    ck_assert(json_string != NULL);
    ck_assert(FAB_JSON_TRUE == json_string->token.type);
    ck_assert_ptr_eq(NULL, json_string->next);

    fab_json_destroy(json);
}

void test_array()
{
    int i;
    fab_json_t json = NULL;
    json_string_clear();

    json = fab_json_create(json_string_append, NULL);

    ck_assert(json != NULL);

    for (i = 0; i < 4; i++)
        fab_json_process(json, "[,,]"[i]);


    struct json_string_token *token = json_string;
    ck_assert(token != NULL);
    ck_assert_int_eq(FAB_JSON_ARRAY_START, token->token.type);

    token = token->next;
    ck_assert(token != NULL);
    ck_assert_int_eq(FAB_JSON_ENTRY_SEP, token->token.type);

    token = token->next;
    ck_assert(token != NULL);
    ck_assert_int_eq(FAB_JSON_ENTRY_SEP, token->token.type);

    token = token->next;
    ck_assert(token != NULL);
    ck_assert_int_eq(FAB_JSON_ARRAY_END, token->token.type);

    token = token->next;
    ck_assert_ptr_eq(NULL, token);

    fab_json_destroy(json);
}

void test_array2()
{
    int i;
    fab_json_t json = NULL;
    const char *input = "[false,true,null]";
    json_string_clear();

    json = fab_json_create(json_string_append, NULL);

    ck_assert(json != NULL);

    for (i = 0; i < strlen(input); i++)
        fab_json_process(json, input[i]);


    struct json_string_token *token = json_string;
    ck_assert(token != NULL);
    ck_assert_int_eq(FAB_JSON_ARRAY_START, token->token.type);

    token = token->next;
    ck_assert(token != NULL);
    ck_assert_int_eq(FAB_JSON_FALSE, token->token.type);

    token = token->next;
    ck_assert(token != NULL);
    ck_assert_int_eq(FAB_JSON_ENTRY_SEP, token->token.type);

    token = token->next;
    ck_assert(token != NULL);
    ck_assert_int_eq(FAB_JSON_TRUE, token->token.type);

    token = token->next;
    ck_assert(token != NULL);
    ck_assert_int_eq(FAB_JSON_ENTRY_SEP, token->token.type);

    token = token->next;
    ck_assert(token != NULL);
    ck_assert_int_eq(FAB_JSON_NULL, token->token.type);

    token = token->next;
    ck_assert(token != NULL);
    ck_assert_int_eq(FAB_JSON_ARRAY_END, token->token.type);

    token = token->next;
    ck_assert_ptr_eq(NULL, token);

    fab_json_destroy(json);
}

void test_object()
{
    int i;
    fab_json_t json = NULL;
    json_string_clear();

    json = fab_json_create(json_string_append, NULL);

    ck_assert(json != NULL);

    for (i = 0; i < 5; i++)
        fab_json_process(json, "{:,:}"[i]);


    struct json_string_token *token = json_string;
    ck_assert(token != NULL);
    ck_assert_int_eq(FAB_JSON_OBJECT_START, token->token.type);

    token = token->next;
    ck_assert(token != NULL);
    ck_assert_int_eq(FAB_JSON_KEYVAL_SEP, token->token.type);

    token = token->next;
    ck_assert(token != NULL);
    ck_assert_int_eq(FAB_JSON_ENTRY_SEP, token->token.type);

    token = token->next;
    ck_assert(token != NULL);
    ck_assert_int_eq(FAB_JSON_KEYVAL_SEP, token->token.type);

    token = token->next;
    ck_assert(token != NULL);
    ck_assert_int_eq(FAB_JSON_OBJECT_END, token->token.type);

    token = token->next;
    ck_assert_ptr_eq(NULL, token);

    fab_json_destroy(json);
}

void test_whitespace()
{
    int i;
    fab_json_t json = NULL;
    json_string_clear();

    json = fab_json_create(json_string_append, NULL);

    ck_assert(json != NULL);

    for (i = 0; i < 4; i++)
        fab_json_process(json, " \t\x0a\x0d"[i]);

    ck_assert_ptr_eq(NULL, json_string);
}

Suite *json_test_suite()
{
    Suite *s;
    TCase *tc, *tc_null, *tc_false, *tc_true, *tc_array, *tc_object, *tc_whitespace;

    s = suite_create("json");

    tc = tcase_create("create");
    tcase_add_test(tc, create);
    suite_add_tcase(s, tc);

    tc_null = tcase_create("null");
    tcase_add_test(tc_null, test_null);
    suite_add_tcase(s, tc_null);

    tc_false = tcase_create("false");
    tcase_add_test(tc_false, test_false);
    suite_add_tcase(s, tc_false);

    tc_true = tcase_create("true");
    tcase_add_test(tc_true, test_true);
    suite_add_tcase(s, tc_true);

    tc_array = tcase_create("array");
    tcase_add_test(tc_array, test_array);
    tcase_add_test(tc_array, test_array2);
    suite_add_tcase(s, tc_array);

    tc_object = tcase_create("object");
    tcase_add_test(tc_object, test_object);
    suite_add_tcase(s, tc_object);

    tc_whitespace = tcase_create("whitespace");
    tcase_add_test(tc_whitespace, test_whitespace);
    suite_add_tcase(s, tc_whitespace);

    return s;
}
