
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "ge_test.h"

#include <stdlib.h>

#include <check.h>


int main(int argc, char *argv[])
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ge_test_suite();
    sr = srunner_create(s);
    srunner_set_tap(sr, "-");

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
