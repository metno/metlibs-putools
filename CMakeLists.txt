PROJECT(metlibs-putools)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(MetnoUtils)
INCLUDE(MetnoGTestUtils)
INCLUDE(CheckIncludeFiles)

METNO_PVERSION_DEFINES(PUTOOLS "src/puToolsVersion.h")

SET(PKGS
  metlibs-puctools>=6.0.0
)

PKG_CHECK_MODULES(PC_METLIBS REQUIRED
  ${PKGS}
)

check_include_files("regex.h" HAVE_REGEX_H)

FIND_PACKAGE(Boost COMPONENTS date_time system REQUIRED)

SET(lib_name "metlibs-putools")

METNO_GEN_PKGCONFIG("${lib_name}.pc.in" "${lib_name}.pc"
  "${PKGS}" "" "" "")

CONFIGURE_FILE(puTools.pc.in puTools.pc @ONLY)
INSTALL(FILES ${CMAKE_BINARY_DIR}/puTools.pc DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/pkgconfig")

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)
