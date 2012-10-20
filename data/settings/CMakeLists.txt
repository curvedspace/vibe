configure_file(
    org.hawaii.desktop.settings.in
    ${CMAKE_CURRENT_BINARY_DIR}/org.hawaii.desktop.settings
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.hawaii.desktop.settings
        DESTINATION ${CMAKE_INSTALL_SETTINGSDIR})
