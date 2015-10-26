
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <dejagnu.h>

static char buffer[512];

#define TEST_FUNC(func)                             \
void fab_##func(const char *fmt, ...)               \
{                                                   \
    va_list ap;                                     \
                                                    \
    va_start (ap, fmt);                             \
    vsnprintf (buffer, sizeof (buffer), fmt, ap);   \
    va_end (ap);                                    \
                                                    \
    func("%s", buffer);                             \
}

TEST_FUNC(pass);
TEST_FUNC(xpass);
TEST_FUNC(fail);
TEST_FUNC(xfail);
TEST_FUNC(untested);
TEST_FUNC(unresolved);
TEST_FUNC(note);

void fab_totals()
{
    totals();
}


