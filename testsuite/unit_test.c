
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>

#include <check.h>

START_TEST(mytest)
{
    ck_assert(1);
}
END_TEST

START_TEST(mytest2)
{
    ck_assert(1);
}
END_TEST

Suite * ge_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("GE");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, mytest);
    tcase_add_test(tc_core, mytest2);
    suite_add_tcase(s, tc_core);

    return s;
}


int main(int argc, char *argv[])
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ge_suite();
    sr = srunner_create(s);
    srunner_set_tap(sr, "-");

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
