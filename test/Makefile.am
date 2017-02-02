
SUBDIRS = gtest .

if HAVE_GTEST

check_PROGRAMS = checks

checks_SOURCES = check-miClock.cc \
	check-miString.cc \
	check-miStringBuilder.cc \
	check-TimeFilter.cc \
	check-mi_boost_compatibility.cc \
	check-MinMax.cc \
	check-mathalgo.cc
checks_CPPFLAGS = -I$(top_srcdir)/src \
	$(PUCTOOLS_CPPFLAGS) $(BOOST_CPPFLAGS) $(gtest_CPPFLAGS)
checks_LDFLAGS = \
	$(PUCTOOLS_LDFLAGS) $(BOOST_LDFLAGS)  $(gtest_LDFLAGS)
checks_LDADD = -lgtest_main $(top_builddir)/src/libpuTools.la \
	$(PUCTOOLS_LIBS) $(BOOST_LIBS)  $(gtest_LIBS)

TESTS = ${check_PROGRAMS}
endif # HAVE_GTEST

# see https://lists.gnu.org/archive/html/automake/2013-06/msg00051.html
display-testsuite-logs:
	for f in $(TEST_LOGS); do printf "\n=== $$f ===\n\n"; cat $$f; done
.PHONY: display-testsuite-logs
