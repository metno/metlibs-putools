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
	$(CURL_CPPFLAGS)

libpuTools_la_LDFLAGS = ${VERSION_INFO} 
#	$(CURL_LDFLAGS)

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
	miTime.h \
	minmax.h \
	puAlgo.h \
	puMathAlgo.h \
	ttycols.h \
	miSetupParser.h

check_PROGRAMS = \
	check-miClock \
	check-miString

check_miClock_SOURCES = check-miClock.cc
check_miClock_CPPFLAGS = -I$(top_srcdir) $(CURL_CPPFLAGS)
check_miClock_LDFLAGS = $(CURL_LDFLAGS)
check_miClock_LDADD = libpuTools.la $(CURL_LIBS)

check_miString_SOURCES = check-miString.cc
check_miString_CPPFLAGS = -I$(top_srcdir) $(CURL_CPPFLAGS)
check_miString_LDFLAGS = $(CURL_LDFLAGS)
check_miString_LDADD = libpuTools.la $(CURL_LIBS)

TESTS = ${check_PROGRAMS}
