#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([mdp], [0.93.0], [michael@myjm.de])
AC_CONFIG_SRCDIR([include/bitops.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE
LT_PREREQ([2.2])
LT_INIT([dlopen])
AC_CONFIG_MACRO_DIR([m4])


# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([locale.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AM_PROG_CC_C_O

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove setlocale strchr strerror])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
