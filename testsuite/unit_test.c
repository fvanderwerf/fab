
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "ge_test.h"

#include "fab_test.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    test_ge();
    test_cge_true();
    test_cge_false();
    test_cge_null_null();
    test_cge_null_nonnull();
    test_cge_neg_neg();
    test_cge_neg_nonneg();

    fab_totals();
    exit(EXIT_SUCCESS);
}
