
pkgname = puTools

lib_LTLIBRARIES = libpuTools.la

libpuTools_la_SOURCES = \
	miClock.cc \
	miCommandLine.cc \
	miDate.cc \
	miDirtools.cc \
	miString.cc \
	miTime.cc \
	puMathAlgo.cc \
	ttycols.cc \
	TimeFilter.cc

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
	puToolsVersion.h \
	mi_boost_compatibility.hh

libpuTools_la_CPPFLAGS = -fPIC \
	$(PUCTOOLS_CPPFLAGS) $(BOOST_CPPFLAGS)

libpuTools_la_LDFLAGS = -fPIC ${VERSION_INFO}
if ENABLE_SHARED_LIBRARY
libpuTools_la_LDFLAGS += \
	$(PUCTOOLS_LDFLAGS) $(BOOST_LDFLAGS)
libpuTools_la_LIBADD = \
	$(PUCTOOLS_LIBS) $(BOOST_LIBS)
endif

INSTALL=install -p # tells install to preserve timestamp
