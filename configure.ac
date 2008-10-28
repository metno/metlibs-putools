#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#
AC_PREREQ([2.59])
AC_INIT([puTools], [3.1.0], [diana@met.no])
AM_INIT_AUTOMAKE
METNO_SET_VERSION_INFO

AC_CONFIG_SRCDIR([src/miClock.cc])
AC_CONFIG_HEADER([include/config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

AC_LANG_PUSH(C++)

# Checks for libraries.
AC_CXX_HAVE_STD
AC_CXX_HAVE_STL

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

AC_LANG_POP(C++)

#making our own .pc file, checking that it is ok and forfilled
AC_CONFIG_FILES(putools.pc)
AC_OUTPUT

export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:.
PKG_CHECK_MODULES([own_dependencies], [${PACKAGE_TARNAME}])

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 src/Makefile])
AC_OUTPUT
