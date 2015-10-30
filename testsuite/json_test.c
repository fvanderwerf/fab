
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "json_test.h"

#include <json.h>

#include <stdlib.h>

struct json_string_token {
    struct fab_json_token token;
    struct json_string_token *next;
} json_string_token;

struct json_string_token *json_string = NULL;

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

    json = fab_json_create(json_string_append, NULL);

    ck_assert(json != NULL);

    fab_json_process(json, 'n');
    fab_json_process(json, 'u');
    fab_json_process(json, 'l');
    fab_json_process(json, 'l');

    ck_assert(FAB_JSON_NULL == json_string->token.type);
    ck_assert(json_string != NULL);
    ck_assert_ptr_eq(NULL, json_string->next);

    fab_json_destroy(json);
}


Suite *json_test_suite()
{
    Suite *s;
    TCase *tc, *tc_null;

    s = suite_create("json");

    tc = tcase_create("create");
    tcase_add_test(tc, create);
    suite_add_tcase(s, tc);

    tc_null = tcase_create("null");
    tcase_add_test(tc_null, test_null);
    suite_add_tcase(s, tc_null);

    return s;
}
