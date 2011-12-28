add_definitions(-DVIBE_BUILDING ${QT_DEFINITIONS} -Wno-multichar)

include_directories(
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTXML_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
    settingsschema.cpp
    settingsschemaloader.cpp
)

set(HEADERS
    settingsschema.h
    settingsschemaloader.h
)

automoc4(VibeSettingsPrivate SOURCES)
add_library(VibeSettingsPrivate STATIC ${SOURCES})
target_link_libraries(VibeSettingsPrivate ${QT_QTCORE_LIBRARIES} ${QT_QTXML_LIBRARIES})
