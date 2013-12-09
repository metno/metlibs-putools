# $Id$
#AM_CPPFLAGS = -I$(top_srcdir) 
pkgname = puTools

include $(top_srcdir)/metlibs.mk

lib_LTLIBRARIES = libpuTools.la

libpuTools_la_SOURCES = \
	miClock.cc \
	miCommandLine.cc \
	miDate.cc \
	miDirtools.cc \
	miString.cc \
	miTime.cc  \
	ttycols.cc \
	miSetupParser.cc

libpuTools_la_CPPFLAGS = \
	$(CURL_CPPFLAGS) \
	-I$(top_srcdir)

libpuTools_la_LDFLAGS = ${puTools_VERSION_INFO}
if ENABLE_SHARED_LIBRARY
libpuTools_la_LDFLAGS += \
	$(CURL_LDFLAGS)
libpuTools_la_LIBADD = \
	$(CURL_LIBS)
endif

pkg_includedir = ${includedir}/puTools

pkg_include_HEADERS = \
	StlContainerUtil.h \
	miClock.h \
	miCommandLine.h \
	miDate.h \
	miDirtools.h \
	miRing.h \
	miSort.h \
	miString.h \
	miStringBuilder.h \
	miStringFunctions.h \
	miTime.h \
	minmax.h \
	puAlgo.h \
	puMathAlgo.h \
	ttycols.h \
	miSetupParser.h \
	mi_boost_compatibility.hh

check_PROGRAMS = checks

checks_SOURCES = check-miClock.cc check-miString.cc check-miStringBuilder.cc \
	check-miSetupParser.cc check-mi_boost_compatibility.cc
checks_CPPFLAGS = $(gtest_CFLAGS) \
	$(CURL_CPPFLAGS) \
	-I$(top_srcdir)
checks_LDFLAGS = $(gtest_LDFLAGS) \
	$(CURL_LDFLAGS)
checks_LDADD = -lgtest_main libpuTools.la $(gtest_LIBS) -lpthread \
	$(CURL_LIBS)

TESTS = ${check_PROGRAMS}
