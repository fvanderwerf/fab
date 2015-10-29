#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([fab], [0.1], [fvanderwerf@gmail.com])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE
LT_INIT
AC_CONFIG_SRCDIR([include/ge.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P

PKG_CHECK_MODULES([CHECK], [check >= 0.9.12])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 src/Makefile
                 testsuite/Makefile])

AC_REQUIRE_AUX_FILE([tap-driver.sh])

AC_OUTPUT
