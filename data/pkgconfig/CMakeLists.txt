configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/QubeCore.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/QubeCore.pc"
    IMMEDIATE @ONLY)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/QubeGui.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/QubeGui.pc"
    IMMEDIATE @ONLY)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/QubeHardware.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/QubeHardware.pc"
    IMMEDIATE @ONLY)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/QubeStorage.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/QubeStorage.pc"
    IMMEDIATE @ONLY)

set(PC_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/QubeCore.pc
    ${CMAKE_CURRENT_BINARY_DIR}/QubeGui.pc
    ${CMAKE_CURRENT_BINARY_DIR}/QubeHardware.pc
    ${CMAKE_CURRENT_BINARY_DIR}/QubeStorage.pc
)

install(FILES ${PC_FILES}
    DESTINATION lib/pkgconfig COMPONENT Devel)
