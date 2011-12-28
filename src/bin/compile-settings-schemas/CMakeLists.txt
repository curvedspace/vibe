include_directories(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(-Wno-multichar)

set(SOURCES
    main.cpp
    settingsschema.cpp
    settingsschemaloader.cpp
)

automoc4(compile-settings-schemas SOURCES)
add_executable(compile-settings-schemas ${SOURCES})
target_link_libraries(compile-settings-schemas ${QT_QTCORE_LIBRARIES} ${QT_QTXML_LIBRARIES})
