configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/VibeCore.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/VibeCore.pc"
    IMMEDIATE @ONLY)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/VibeGui.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/VibeGui.pc"
    IMMEDIATE @ONLY)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/VibeHardware.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/VibeHardware.pc"
    IMMEDIATE @ONLY)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/VibeStorage.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/VibeStorage.pc"
    IMMEDIATE @ONLY)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/VibeDeclarative.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/VibeDeclarative.pc"
    IMMEDIATE @ONLY)

set(PC_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/VibeCore.pc
    ${CMAKE_CURRENT_BINARY_DIR}/VibeGui.pc
    ${CMAKE_CURRENT_BINARY_DIR}/VibeHardware.pc
    ${CMAKE_CURRENT_BINARY_DIR}/VibeStorage.pc
    ${CMAKE_CURRENT_BINARY_DIR}/VibeDeclarative.pc
)

install(FILES ${PC_FILES}
    DESTINATION ${CMAKE_INSTALL_DEVELOPDIR}/pkgconfig COMPONENT Devel)
