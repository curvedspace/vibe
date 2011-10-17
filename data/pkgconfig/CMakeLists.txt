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

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/QubeCore.pc"
    DESTINATION lib/pkgconfig COMPONENT Devel)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/QubeGui.pc"
    DESTINATION lib/pkgconfig COMPONENT Devel)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/QubeHardware.pc"
    DESTINATION lib/pkgconfig COMPONENT Devel)
