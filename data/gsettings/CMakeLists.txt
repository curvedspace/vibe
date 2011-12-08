set(FILES
    org.vision.desktop.enums.xml
    org.vision.desktop.interface.xml
)

install(FILES ${FILES}
    DESTINATION ${CMAKE_INSTALL_DATADIR}/glib-2.0/schemas COMPONENT Framework)
