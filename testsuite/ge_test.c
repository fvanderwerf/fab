
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "ge_test.h"

#include <ge.h>


void test_ge()
{
    FAB_GE();
    ck_abort();
error:
    return;
}

void test_cge_true()
{
    FAB_CGE(1);
    ck_abort();
error:
    return;
}

void test_cge_false()
{
    FAB_CGE(0);
    return;
error:
    ck_abort();
    return;
}
void test_cge_null_null()
{
    FAB_CGE_NULL(NULL);
    ck_abort();
error:
    return;
}

void test_cge_null_nonnull()
{
    FAB_CGE_NULL((int *) 1);
    return;
error:
    ck_abort();
    return;
}

void test_cge_neg_neg()
{
    FAB_CGE_NEG(-1);
    ck_abort();
error:
    return;
}

void test_cge_neg_nonneg()
{
    FAB_CGE_NEG(0);
    FAB_CGE_NEG(1);
    return;
error:
    ck_abort();
    return;
}

Suite *ge_test_suite()
{
    Suite *s;
    TCase *tc_ge, *tc_cge, *tc_cge_null, *tc_cge_neg;

    s = suite_create("GE");

    /* ge test case */
    tc_ge = tcase_create("GE");
    tcase_add_test(tc_ge, test_ge);
    suite_add_tcase(s, tc_ge);

    /* cge test case */
    tc_cge = tcase_create("CGE");
    tcase_add_test(tc_cge, test_cge_true);
    tcase_add_test(tc_cge, test_cge_false);
    suite_add_tcase(s, tc_cge);

    /* cge_null test case */
    tc_cge_null = tcase_create("CGE_NULL");
    tcase_add_test(tc_cge_null, test_cge_null_null);
    tcase_add_test(tc_cge_null, test_cge_null_nonnull);
    suite_add_tcase(s, tc_cge_null);

    /* cge_neg test case */
    tc_cge_neg = tcase_create("CGE_NEG");
    tcase_add_test(tc_cge_neg, test_cge_neg_neg);
    tcase_add_test(tc_cge_neg, test_cge_neg_nonneg);
    suite_add_tcase(s, tc_cge_neg);

    return s;

}
