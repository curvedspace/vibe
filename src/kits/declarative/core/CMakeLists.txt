
include_directories(
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTDECLARATIVE_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
    dialog.cpp
    theme.cpp
)

qt4_automoc(${SOURCES})

add_library(VibeDeclarativeCorePlugin SHARED ${SOURCES})
target_link_libraries(VibeDeclarativeCorePlugin ${QT_QTDECLARATIVE_LIBRARIES})

install(TARGETS VibeDeclarativeCorePlugin ${QT_IMPORTS_DIR}/Vibe/Core)
install(FILES qmldir DESTINATION ${QT_IMPORTS_DIR}/Vibe/Core)
