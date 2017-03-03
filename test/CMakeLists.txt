
INCLUDE_DIRECTORIES(${PC_METLIBS_INCLUDE_DIRS} ${BOOST_INCLUDE_DIRS})
ADD_DEFINITIONS(-W -Wall ${PC_METLIBS_CFLAGS_OTHER})
LINK_DIRECTORIES(${PC_METLIBS_LIBRARY_DIRS} ${BOOST_LIBRARY_DIRS})

METNO_FIND_GTEST()

INCLUDE_DIRECTORIES(
  "${CMAKE_SOURCE_DIR}/src"
)

ADD_EXECUTABLE(putools_test
  check-miClock.cc
  check-miString.cc
  check-miStringBuilder.cc
  check-TimeFilter.cc
  check-MinMax.cc
  check-mathalgo.cc
)

TARGET_LINK_LIBRARIES(putools_test
  putools
  ${GTEST_MAIN_LIBRARY}
)

ADD_TEST(NAME putools_test
  COMMAND putools_test --gtest_color=yes
)
