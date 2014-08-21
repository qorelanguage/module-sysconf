cmake_minimum_required(VERSION 2.8.3)

project(qore-sysconf-module)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)
set (VERSION_PATCH 0)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )


FIND_PACKAGE (Qore REQUIRED)


# detect sysconf function
INCLUDE (CheckFunctionExists)
CHECK_FUNCTION_EXISTS(sysconf HAVE_SYSCONF)
if (HAVE_SYSCONF)
    add_definitions(-DHAVE_SYSCONF)
endif(HAVE_SYSCONF)
CHECK_FUNCTION_EXISTS(confstr HAVE_CONFSTR)
if (HAVE_CONFSTR)
    add_definitions(-DHAVE_CONFSTR)
endif (HAVE_CONFSTR)
CHECK_FUNCTION_EXISTS(pathconf HAVE_PATHCONF)
if (HAVE_PATHCONF)
    add_definitions(-DHAVE_PATHCONF)
endif(HAVE_PATHCONF)

if (${CMAKE_SYSTEM_NAME} MATCHES SunOS)
    add_definitions(-D__EXTENSIONS__)
endif()


include_directories( ${CMAKE_SOURCE_DIR}/src )

set(CPP_SRC
)
set(QPP_SRC
    src/sysconf.qpp
    src/constants_sysconf.qpp
    src/constants_confstr.qpp
    src/constants_pathconf.qpp
)

qore_wrap_qpp(QPP_SOURCES ${QPP_SRC})

SET (module_name "sysconf")

add_library(${module_name} SHARED ${CPP_SRC} ${QPP_SOURCES})

qore_binary_module(${module_name} "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

qore_dist("${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

qore_config_info()

