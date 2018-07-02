#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([ipc-server], [1.0.0], [513829215@qq.com])
AC_CONFIG_SRCDIR([src/ipc-server.cpp])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall, -Werror, foreign])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([getcwd memset])

AAC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CONFIG_FILES([Makefile
                 inc/Makefile
                 src/Makefile])
AC_OUTPUT
