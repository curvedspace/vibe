set(FILES
    org.vision.desktop.enums.xml
    org.vision.desktop.interface.gschema.xml
)

install(FILES ${FILES}
    DESTINATION ${CMAKE_INSTALL_DATADIR}/glib-2.0/schemas COMPONENT Framework)

add_custom_target(glib_compile_schemas
    COMMAND glib-compile-schemas ${CMAKE_INSTALL_DATADIR}/glib-2.0/schemas)
add_dependencies(glib_compile_schemas install)
