add_definitions("-DPACKAGE_VERSION=\"${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}\"")

include_directories( ${QORE_INCLUDE_DIR} )

set(qoresysconf_LIB_SRCS
    sysconf-module.cpp
)

SET (module_name "sysconf")

add_library(${module_name} SHARED ${qoresysconf_LIB_SRCS})
set_target_properties(${module_name} PROPERTIES PREFIX "" SUFFIX "-api-${QORE_API_VERSION}.qmod")
target_link_libraries(${module_name} ${QORE_LIBRARY} )
install( TARGETS ${module_name} DESTINATION ${QORE_MODULES_DIR})
