set(FILES
    org.maui.desktop.background.gschema.xml
    org.maui.desktop.enums.xml
    org.maui.desktop.interface.gschema.xml
)

set(CMAKE_INSTALL_SCHEMASDIR ${CMAKE_INSTALL_DATADIR}/glib-2.0/schemas)
install(FILES ${FILES} DESTINATION ${CMAKE_INSTALL_SCHEMASDIR})

#glib-compile-schemas ${CMAKE_INSTALL_SCHEMASDIR}
