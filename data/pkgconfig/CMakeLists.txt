configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/VibeCore.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/VibeCore.pc"
    IMMEDIATE @ONLY)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/VibeGui.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/VibeGui.pc"
    IMMEDIATE @ONLY)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/VibeWidgets.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/VibeWidgets.pc"
    IMMEDIATE @ONLY)

set(PC_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/VibeCore.pc
    ${CMAKE_CURRENT_BINARY_DIR}/VibeGui.pc
    ${CMAKE_CURRENT_BINARY_DIR}/VibeWidgets.pc
)

install(FILES ${PC_FILES} DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
