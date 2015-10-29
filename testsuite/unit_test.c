
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "ge_test.h"
#include "utf8_test.h"
#include "json_test.h"

#include <stdlib.h>

#include <check.h>

int main(int argc, char *argv[])
{
    int number_failed;
    Suite *sge, *sutf8, *sjson;
    SRunner *sr;

    sge = ge_test_suite();
    sutf8 = utf8_test_suite();
    sjson = json_test_suite();

    sr = srunner_create(sge);
    srunner_add_suite(sr, sutf8);
    srunner_add_suite(sr, sjson);

    srunner_set_tap(sr, "-");

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
