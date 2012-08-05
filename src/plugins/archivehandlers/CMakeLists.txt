include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/headers/VibeCore
    ${CMAKE_BINARY_DIR}/src/core
)

add_definitions(${Qt5Core_DEFINITIONS} -DQT_PLUGIN)

add_library(vtar SHARED tararchivehandler.cpp)
target_link_libraries(vtar VibeCore)

add_library(var SHARED ararchivehandler.cpp)
target_link_libraries(var VibeCore)

install(TARGETS vtar var LIBRARY DESTINATION ${CMAKE_INSTALL_PLUGINSDIR}/archivehandlers)
