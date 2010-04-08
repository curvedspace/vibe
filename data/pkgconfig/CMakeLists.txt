project(QubePkgConfig)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/QubeCore.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/QubeCore.pc"
    IMMEDIATE @ONLY)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/QubeGui.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/QubeGui.pc"
    IMMEDIATE @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/QubeCore.pc"
    DESTINATION ${QT_LIBRARY_DIR}/pkgconfig COMPONENT Devel)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/QubeGui.pc"
    DESTINATION ${QT_LIBRARY_DIR}/pkgconfig COMPONENT Devel)
