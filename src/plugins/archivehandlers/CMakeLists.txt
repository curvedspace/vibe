include_directories(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN -DQT_NO_DEBUG -DQT_SHARED)

set(SOURCES
    tararchivehandler.cpp
    archivesplugin.cpp
)

automoc4(VibeArchivePlugin SOURCES)
add_library(VibeArchivePlugin SHARED ${SOURCES})
target_link_libraries(VibeArchivePlugin VibeCore)

install(TARGETS VibeArchivePlugin LIBRARY
    DESTINATION ${CMAKE_INSTALL_PLUGINSDIR}/archivehandlers COMPONENT ArchiveHandlers)
