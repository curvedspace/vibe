configure_file(
    org.hawaii.desktop.json.in
    ${CMAKE_CURRENT_BINARY_DIR}/org.hawaii.desktop.json
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.hawaii.desktop.json
        DESTINATION ${CMAKE_INSTALL_SETTINGSDIR})
