

LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) $(top_srcdir)/build-aux/tap-driver.sh

TESTS = unit_test

check_PROGRAMS = unit_test
unit_test_SOURCES = unit_test.c ge_test.c utf8_test.c json_test.c
unit_test_CFLAGS = -I $(top_srcdir)/include @CHECK_CFLAGS@
unit_test_LDADD = ../src/libfab.la @CHECK_LIBS@
