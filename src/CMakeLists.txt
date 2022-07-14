
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${PC_METLIBS_INCLUDE_DIRS} ${BOOST_INCLUDE_DIRS})
ADD_DEFINITIONS(-W -Wall ${PC_METLIBS_CFLAGS_OTHER})
LINK_DIRECTORIES(${PC_METLIBS_LIBRARY_DIRS} ${BOOST_LIBRARY_DIRS})

SET(putools_SOURCES
  miClock.cc
  miCommandLine.cc
  miDate.cc
  miDirtools.cc
  miString.cc
  miTime.cc
  puMathAlgo.cc
  ttycols.cc
  TimeFilter.cc
)

METNO_HEADERS (putools_HEADERS putools_SOURCES ".cc" ".h")
LIST(APPEND putools_HEADERS
  miRing.h
  miSort.h
  miStringBuilder.h
  miStringFunctions.h
  minmax.h
  puAlgo.h
  puToolsVersion.h
)

configure_file(config.h.in config.h @ONLY)

########################################################################

ADD_LIBRARY(putools SHARED
  ${putools_SOURCES}
)

SET_TARGET_PROPERTIES(putools PROPERTIES
  VERSION ${PUTOOLS_PVERSION_FULL}
  SOVERSION "${PUTOOLS_VERSION_MAJOR}"
  OUTPUT_NAME "${lib_name}"
)

TARGET_LINK_LIBRARIES(putools
  ${Boost_LIBRARIES}
)

INSTALL(TARGETS putools
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

INSTALL(FILES ${putools_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/metlibs/puTools
)
