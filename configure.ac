#
# configure.ac for puTools
#

AC_PREREQ(2.61)
# version number must be in sync with src/puToolsVersion.h
AC_INIT([puTools], [5.1.13], [diana@met.no])
AM_INIT_AUTOMAKE([foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CONFIG_SRCDIR([src/miString.h])
AC_CONFIG_MACRO_DIR([m4])

## check version consistency
V_HEADER_FILE=${srcdir}/src/puToolsVersion.h
V_MAJOR=`grep '^#define METLIBS_PUTOOLS_VERSION_MAJOR' "${V_HEADER_FILE}" | cut -d' ' -f3`
V_MINOR=`grep '^#define METLIBS_PUTOOLS_VERSION_MINOR' "${V_HEADER_FILE}" | cut -d' ' -f3`
V_PATCH=`grep '^#define METLIBS_PUTOOLS_VERSION_PATCH' "${V_HEADER_FILE}" | cut -d' ' -f3`
if echo "$VERSION" | grep -q "^${V_MAJOR}\\.${V_MINOR}\\.${V_PATCH}\\($\\|[^0-9]\\)" ; then
  AC_MSG_NOTICE([found expected version ${VERSION} in ${V_HEADER_FILE}])
else
  AC_MSG_ERROR([expected version $VERSION but found ${V_MAJOR}.${V_MINOR}.${V_PATCH} in ${V_HEADER_FILE}])
fi

METNO_SET_VERSION_INFO(6.0.0)
METNO_PROG_PKG_CONFIG

AC_PROG_LIBTOOL
AC_SUBST([LIBTOOL_DEPS]) 

AC_PROG_CXX

AC_LANG_PUSH(C++)
AC_CXX_HAVE_STD
AC_CXX_HAVE_STL
AC_LANG_POP([C++])


METNO_ENABLE_CONDITIONAL([SHARED_LIBRARY], [shared])

METNO_REQUIRE_PKG([PUCTOOLS],[puCtools])
METNO_REQUIRE_BOOST
METNO_REQUIRE_BOOST_FEATURE([date_time])

GTEST_CHECK([test/gtest])

AC_CONFIG_FILES([
	puTools.pc
	Makefile
	src/Makefile
	test/Makefile
	test/gtest/Makefile
])

AC_OUTPUT
