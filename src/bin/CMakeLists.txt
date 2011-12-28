include_directories(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/src/kits/core
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(lshw_SOURCES lshw.cpp)
automoc4(lshw lshw_SOURCES)
add_executable(lshw ${lshw_SOURCES})
target_link_libraries(lshw VibeCore VibeHardware)
install(TARGETS lshw
    DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Binaries)

set(compile-settings-schemas_SOURCES compile-settings-schemas.cpp)
automoc4(compile-settings-schemas compile-settings-schemas_SOURCES)
add_executable(compile-settings-schemas ${compile-settings-schemas_SOURCES})
target_link_libraries(compile-settings-schemas VibeCore)
install(TARGETS compile-settings-schemas
    DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Binaries)
