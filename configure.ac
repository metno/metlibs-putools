#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([puTools], [4.0.0], [diana@met.no])
AM_INIT_AUTOMAKE
MI_SET_VERSION_INFO

AC_CONFIG_HEADER([include/config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_LANG(C++)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_DIRENT
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

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
	         putools.pc
		 include/Makefile
		 src/Makefile])

AC_OUTPUT
