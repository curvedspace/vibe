include(CMakePackageConfigHelpers)

macro(produce_cmake_script _module_name _module_deps)
    set(INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Vibe${_module_name})

    configure_package_config_file(VibeBasicConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/Vibe${_module_name}Config.cmake
                                  INSTALL_DESTINATION ${INSTALL_CMAKEDIR}
                                  PATH_VARS INSTALL_CMAKEDIR)
    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/Vibe${_module_name}ConfigVersion.cmake
                                     VERSION ${VIBE_VERSION_STRING}
                                     COMPATIBILITY AnyNewerVersion)

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Vibe${_module_name}Config.cmake
                  ${CMAKE_CURRENT_BINARY_DIR}/Vibe${_module_name}ConfigVersion.cmake
            DESTINATION ${INSTALL_CMAKEDIR})
endmacro()

produce_cmake_script("Core" "Qt5Core;Qt5DBus;Qt5Xml")
produce_cmake_script("Gui" "Qt5Gui;VibeCore")
produce_cmake_script("Widgets" "Qt5Gui;Qt5Widgets;VibeGui;solid")
