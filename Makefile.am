# $Id: Makefile.am 2786 2009-09-23 11:39:02Z dages $

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

libpuTools_la_LDFLAGS = \
	${VERSION_INFO}

pkg_includedir = ${prefix}/include/puTools

pkg_include_HEADERS = \
	StlContainerUtil.h \
	miClock \
	miClock.h \
	miCommandLine.h \
	miDate \
	miDate.h \
	miDirtools.h \
	miRing.h \
	miSort.h \
	miString \
	miString.h \
	miTime \
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
check_miClock_LDADD = libpuTools.la

check_miString_SOURCES = check-miString.cc
check_miString_LDADD = libpuTools.la

TESTS = ${check_PROGRAMS}
