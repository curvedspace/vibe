include_directories(
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/headers/VibeCore
    ${CMAKE_BINARY_DIR}/src/core
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Sources
set(SOURCES
    vcolorscheme.cpp
)

# Headers
set(HEADERS
    vcolorscheme.h
)

# Add library
add_compiler_export_flags()
add_library(VibeGui SHARED ${SOURCES})
generate_export_header(VibeGui
    BASE_NAME vibegui
    DEPRECATED_MACRO_NAME VIBE_DEPRECATED
)
qt5_use_modules(VibeGui Gui)
target_link_libraries(VibeGui VibeCore)
set_target_properties(VibeGui PROPERTIES
    VERSION ${VIBE_VERSION_STRING}
    SOVERSION ${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_PATCH})

# Install files
set(HEADERS ${HEADERS} ${CMAKE_CURRENT_BINARY_DIR}/vibegui_export.h)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeGui)
install(TARGETS VibeGui LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
