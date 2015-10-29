
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "json_test.h"

#include <json.h>

#include <stdlib.h>

void create()
{
    fab_json_t json = NULL;

    json = fab_json_create();

    ck_assert(json != NULL);

    fab_json_destroy(json);
}


Suite *json_test_suite()
{
    Suite *s;
    TCase *tc;

    s = suite_create("json");

    tc = tcase_create("create");
    tcase_add_test(tc, create);
    suite_add_tcase(s, tc);

    return s;
}
