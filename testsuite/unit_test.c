
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>

#include <ge.h>

#include <dejagnu.h>

void test_ge()
{
    FAB_GE();
    fail(__func__);
    return;
error:
    pass(__func__);
    return;
}

void test_cge_true()
{
    FAB_CGE(1);
    fail(__func__);
    return;
error:
    pass(__func__);
    return;
}

void test_cge_false()
{
    FAB_CGE(0);
    pass(__func__);
    return;
error:
    fail(__func__);
    return;
}

void test_cge_null_null()
{
    FAB_CGE_NULL(NULL);
    fail(__func__);
    return;
error:
    pass(__func__);
    return;
}

void test_cge_null_nonnull()
{
    FAB_CGE_NULL((int *) 1);
    pass(__func__);
    return;
error:
    fail(__func__);
    return;
}

void test_cge_neg_neg()
{
    FAB_CGE_NEG(-1);
    fail(__func__);
    return;
error:
    pass(__func__);
    return;
}

void test_cge_neg_nonneg()
{
    FAB_CGE_NEG(0);
    FAB_CGE_NEG(1);
    pass(__func__);
    return;
error:
    fail(__func__);
    return;
}

int main(int argc, char *argv[])
{
    test_ge();
    test_cge_true();
    test_cge_false();
    test_cge_null_null();
    test_cge_null_nonnull();
    test_cge_neg_neg();
    test_cge_neg_nonneg();

    totals();
    exit(EXIT_SUCCESS);
}
