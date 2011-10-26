#
# to use the file to build a Qt only version of PHP-Qt, copy it to CMakeLists.txt
#
# make a build/ dir, step into
# run: cmake ..
# run: make
#
# for further information read the README file in php/phpqt
#
cmake_minimum_required(VERSION 2.6)

project(qore-sysconf-module)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)
set (VERSION_PATCH 0)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )

if (ENABLE_DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
    add_definitions("-DDEBUG=1")
else (ENABLE_DEBUG)
    set(CMAKE_BUILD_TYPE Release)
endif (ENABLE_DEBUG)


SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")


# make uninstall
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY
)
ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")


SET(QORE_MIN_VERSION "0.8.0")
FIND_PACKAGE (Qore REQUIRED)

FIND_PACKAGE (Doxygen)

# detect sysconf function
INCLUDE (CheckFunctionExists)
CHECK_FUNCTION_EXISTS(sysconf HAVE_SYSCONF)
if (HAVE_SYSCONF)
    add_definitions(-DHAVE_SYSCONF)
endif(HAVE_SYSCONF)


MESSAGE(STATUS "")
MESSAGE(STATUS "-----------------------------")
MESSAGE(STATUS "System: ${CMAKE_SYSTEM}")
IF (APPLE)
    MESSAGE(STATUS "Archs: ${CMAKE_OSX_ARCHITECTURES}")
ENDIF (APPLE)
MESSAGE(STATUS "Compiler: ${CMAKE_CXX_COMPILER} - ${CMAKE_CXX_COMPILER_ID}")
MESSAGE(STATUS "FLags deb: ${CMAKE_CXX_FLAGS_DEBUG}")
MESSAGE(STATUS "FLags rel: ${CMAKE_CXX_FLAGS}")
MESSAGE(STATUS "-----------------------------")


add_subdirectory(src)
if (DOXYGEN_FOUND)
    add_subdirectory(docs)
endif (DOXYGEN_FOUND)


# packaging related stuff
string(TOLOWER ${CMAKE_PROJECT_NAME} CPACK_PACKAGE_NAME)
SET(CPACK_PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
SET(CPACK_SOURCE_GENERATOR "TBZ2")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
SET(CPACK_IGNORE_FILES "/CVS/;/\\\\.svn/;\\\\.swp$;\\\\.#;/#;\\\\.~$;\\\\.tar.gz$;/CMakeFiles/;CMakeCache.txt;refresh-copyright-and-license.pl;\\\\.spec$")
SET(CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES})
INCLUDE(CPack)
# simulate autotools' "make dist"
add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)
