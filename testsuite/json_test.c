
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "json_test.h"

#include <json.h>

#include <string.h>
#include <stdlib.h>
#include <math.h>

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

static fab_json_t json = NULL;

static void setup()
{
    json = fab_json_create(json_string_append, NULL);

    ck_assert(json != NULL);
}

static void teardown()
{
    json_string_clear();
    fab_json_destroy(json);
    json = NULL;
}

START_TEST(test_null)
{
    fab_json_process(json, 'n');
    fab_json_process(json, 'u');
    fab_json_process(json, 'l');
    fab_json_process(json, 'l');

    ck_assert(json_string != NULL);
    ck_assert(FAB_JSON_NULL == json_string->token.type);
    ck_assert_ptr_eq(NULL, json_string->next);
}
END_TEST

START_TEST(test_false)
{
    const char *input = "false";
    size_t input_length = strlen(input);
    int i;

    for (i = 0; i < input_length; i++)
        fab_json_process(json, input[i]);

    ck_assert(json_string != NULL);
    ck_assert(FAB_JSON_FALSE == json_string->token.type);
    ck_assert_ptr_eq(NULL, json_string->next);
}
END_TEST


START_TEST(test_true)
{
    const char *input = "true";
    size_t input_length = strlen(input);
    int i;

    for (i = 0; i < input_length; i++)
        fab_json_process(json, input[i]);

    ck_assert(json_string != NULL);
    ck_assert(FAB_JSON_TRUE == json_string->token.type);
    ck_assert_ptr_eq(NULL, json_string->next);
}
END_TEST

START_TEST(test_array)
{
    const char *input = "[,,]";
    size_t input_length = strlen(input);
    int i;

    for (i = 0; i < input_length; i++)
        fab_json_process(json, input[i]);

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
}
END_TEST

START_TEST(test_array2)
{
    const char *input = "[false,true,null]";
    size_t input_length = strlen(input);
    int i;

    for (i = 0; i < input_length; i++)
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
}
END_TEST

START_TEST(test_object)
{
    const char *input = "{:,:}";
    size_t input_length = strlen(input);
    int i;

    for (i = 0; i < input_length; i++)
        fab_json_process(json, input[i]);

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
}
END_TEST

START_TEST(test_whitespace)
{
    const char *input = " \t\x0a\x0d";
    size_t input_length = strlen(input);
    int i;

    for (i = 0; i < input_length; i++)
        fab_json_process(json, input[i]);

    ck_assert_ptr_eq(NULL, json_string);
}
END_TEST

START_TEST(test_number_int)
{
    int i;
    const char *input = "42 ";
    size_t input_length = strlen(input);

    for (i = 0; i < input_length; i++)
        fab_json_process(json, input[i]);

    struct json_string_token *token = json_string;
    ck_assert(token != NULL);
    ck_assert_int_eq(FAB_JSON_NUMBER, token->token.type);
    ck_assert_int_eq(42, round(token->token.value.number));

    token = token->next;
    ck_assert_ptr_eq(NULL, token);
}
END_TEST

Suite *json_test_suite()
{
    Suite *s;
    TCase *tc, *tc_null, *tc_bool, *tc_array, *tc_object,
          *tc_whitespace, *tc_number;

    s = suite_create("json");

    tc = tcase_create("create");
    tcase_add_test(tc, create);
    suite_add_tcase(s, tc);

    tc_null = tcase_create("null");
    tcase_add_checked_fixture(tc_null, setup, teardown);
    tcase_add_test(tc_null, test_null);
    suite_add_tcase(s, tc_null);

    tc_bool = tcase_create("boolean");
    tcase_add_checked_fixture(tc_bool, setup, teardown);
    tcase_add_test(tc_bool, test_false);
    tcase_add_test(tc_bool, test_true);
    suite_add_tcase(s, tc_bool);

    tc_array = tcase_create("array");
    tcase_add_checked_fixture(tc_array, setup, teardown);
    tcase_add_test(tc_array, test_array);
    tcase_add_test(tc_array, test_array2);
    suite_add_tcase(s, tc_array);

    tc_object = tcase_create("object");
    tcase_add_checked_fixture(tc_object, setup, teardown);
    tcase_add_test(tc_object, test_object);
    suite_add_tcase(s, tc_object);

    tc_whitespace = tcase_create("whitespace");
    tcase_add_checked_fixture(tc_whitespace, setup, teardown);
    tcase_add_test(tc_whitespace, test_whitespace);
    suite_add_tcase(s, tc_whitespace);

    tc_number = tcase_create("number");
    tcase_add_checked_fixture(tc_number, setup, teardown);
    tcase_add_test(tc_number, test_number_int);
    suite_add_tcase(s, tc_number);

    return s;
}
