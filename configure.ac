#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(putools, 4.0.0, dina@met.no)
AC_CONFIG_SRCDIR([src/miString.cc])
MI_SET_VERSION_INFO

AM_INIT_AUTOMAKE

AC_CONFIG_HEADER([include/config.h])
AC_CONFIG_MACRO_DIR([m4])


# Checks for programs.
AC_PROG_YACC
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LEX
AC_LANG(C++)

AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_DIRENT
AC_CHECK_HEADERS([stdlib.h string.h unistd.h], ,AC_MSG_ERROR("missing needed header file"))

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_ERROR_AT_LINE
AC_FUNC_STAT
AC_CHECK_FUNCS([floor getcwd regcmp regcomp])

AC_CONFIG_FILES([Makefile
		 src/Makefile])

AC_OUTPUT
