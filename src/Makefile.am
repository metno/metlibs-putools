
pkgname = puTools

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

pkg_includedir = ${includedir}/puTools

pkg_include_HEADERS = \
	$(libpuTools_la_SOURCES:.cc=.h) \
	StlContainerUtil.h \
	miRing.h \
	miSort.h \
	miStringBuilder.h \
	miStringFunctions.h \
	minmax.h \
	puAlgo.h \
	puMathAlgo.h \
	mi_boost_compatibility.hh

libpuTools_la_CPPFLAGS = \
	$(CURL_CPPFLAGS) $(PUCTOOLS_CPPFLAGS) $(BOOST_CPPFLAGS)

libpuTools_la_LDFLAGS = ${VERSION_INFO}
if ENABLE_SHARED_LIBRARY
libpuTools_la_LDFLAGS += \
	$(CURL_LDFLAGS)  $(PUCTOOLS_LDFLAGS) $(BOOST_LDFLAGS)
libpuTools_la_LIBADD = \
	$(CURL_LIBS) $(PUCTOOLS_LIBS) $(BOOST_LIBS)
endif

if HAVE_GTEST
check_PROGRAMS = checks

checks_SOURCES = check-miClock.cc check-miString.cc check-miStringBuilder.cc \
	check-miSetupParser.cc check-mi_boost_compatibility.cc
checks_CPPFLAGS = -I$(srcdir) \
	$(CURL_CPPFLAGS) $(PUCTOOLS_CPPFLAGS) $(BOOST_CPPFLAGS) $(gtest_CPPFLAGS)
checks_LDFLAGS = \
	$(CURL_LDFLAGS)  $(PUCTOOLS_LDFLAGS) $(BOOST_LDFLAGS)  $(gtest_LDFLAGS)
checks_LDADD = -lgtest_main libpuTools.la \
	$(CURL_LIBS) $(PUCTOOLS_LIBS) $(BOOST_LIBS)  $(gtest_LIBS)

TESTS = ${check_PROGRAMS}
endif # HAVE_GTEST
