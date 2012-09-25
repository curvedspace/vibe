set(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/org.hawaii.desktop.background.gschema.xml
    org.hawaii.desktop.enums.xml
    org.hawaii.desktop.interface.gschema.xml
)

configure_file(
    org.hawaii.desktop.background.gschema.xml.in
    ${CMAKE_CURRENT_BINARY_DIR}/org.hawaii.desktop.background.gschema.xml
)

# Check if pkg-config is available so that
# we have PKG_CONFIG_EXECUTABLE define
find_package(PkgConfig "0.22" REQUIRED)

# Find gio prefix
execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=prefix gio-2.0
    OUTPUT_VARIABLE GLIB_PREFIX
    RESULT_VARIABLE _failed
    OUTPUT_STRIP_TRAILING_WHITESPACE)
if(_failed)
    message(FATAL_ERROR "Cannot find prefix propery from gio-2.0: ${_failed}")
endif(_failed)

# Find glib-compile-schemas
execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=glib_compile_schemas gio-2.0
    OUTPUT_VARIABLE GLIB_COMPILE_SCHEMAS
    RESULT_VARIABLE _failed
    OUTPUT_STRIP_TRAILING_WHITESPACE)
if(_failed)
    message(FATAL_ERROR "Cannot find glib_compile_schemas propery from gio-2.0: ${_failed}")
endif(_failed)

# If gio was installed in another path we assume it's a standard Linux FHS
# and try a hard-coded path, this is not elegant, I wish gio-2.0.pc had a
# datadir property
set(INSTALL_SCHEMASDIR ${GLIB_PREFIX}/data/glib-2.0/schemas)
if(NOT EXISTS ${INSTALL_SCHEMASDIR})
    set(INSTALL_SCHEMASDIR ${GLIB_PREFIX}/share/glib-2.0/schemas)
    if(NOT EXISTS ${INSTALL_SCHEMASDIR})
        set(INSTALL_SCHEMASDIR ${CMAKE_INSTALL_DATADIR}/glib-2.0/schemas)
    endif()
endif()

# Install schemas and compile them during the post-installation phase
configure_file(PostInstall.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/PostInstall.cmake)
install(FILES ${FILES} DESTINATION ${INSTALL_SCHEMASDIR})
install(SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/PostInstall.cmake")
