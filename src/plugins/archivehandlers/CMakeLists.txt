include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/headers/VibeCore
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(${Qt5Core_CFLAGS} -DQT_PLUGIN)

set(SOURCES
    tararchivehandler.cpp
    archivesplugin.cpp
)

add_library(VibeArchivePlugin SHARED ${SOURCES})
target_link_libraries(VibeArchivePlugin VibeCore)

install(TARGETS VibeArchivePlugin LIBRARY
    DESTINATION ${CMAKE_INSTALL_PLUGINSDIR}/archivehandlers COMPONENT ArchiveHandlers)
