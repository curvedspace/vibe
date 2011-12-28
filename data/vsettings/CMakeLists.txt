set(FILES
    org.vision.desktop.enums.xml
    org.vision.desktop.vss
)

install(FILES ${FILES}
    DESTINATION ${CMAKE_INSTALL_DATADIR}/vsettings/schemas COMPONENT Framework)
