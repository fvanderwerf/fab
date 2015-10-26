
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>

#include <ge.h>

#include <dejagnu.h>

void test_ge()
{
    FAB_GE();
    fail("GE");
    return;
error:
    pass("GE");
    return;
}

int main(int argc, char *argv[])
{
    test_ge();
    totals();
    exit(EXIT_SUCCESS);
}
