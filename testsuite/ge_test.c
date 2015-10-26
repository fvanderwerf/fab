
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <ge.h>

#include "fab_test.h"

void test_ge()
{
    FAB_GE();
    fab_fail(__func__);
    return;
error:
    fab_pass(__func__);
    return;
}

void test_cge_true()
{
    FAB_CGE(1);
    fab_fail(__func__);
    return;
error:
    fab_pass(__func__);
    return;
}

void test_cge_false()
{
    FAB_CGE(0);
    fab_pass(__func__);
    return;
error:
    fab_fail(__func__);
    return;
}

void test_cge_null_null()
{
    FAB_CGE_NULL(NULL);
    fab_fail(__func__);
    return;
error:
    fab_pass(__func__);
    return;
}

void test_cge_null_nonnull()
{
    FAB_CGE_NULL((int *) 1);
    fab_pass(__func__);
    return;
error:
    fab_fail(__func__);
    return;
}

void test_cge_neg_neg()
{
    FAB_CGE_NEG(-1);
    fab_fail(__func__);
    return;
error:
    fab_pass(__func__);
    return;
}

void test_cge_neg_nonneg()
{
    FAB_CGE_NEG(0);
    FAB_CGE_NEG(1);
    fab_pass(__func__);
    return;
error:
    fab_fail(__func__);
    return;
}
