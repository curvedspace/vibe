macro(produce_cmake_script _module_name _module_deps)
    set(CMAKE_MODULE_NAME ${_module_name})
    set(CMAKE_MODULE_DEPS ${_module_deps})

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/VibeBasicConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/Vibe${_module_name}Config.cmake"
        IMMEDIATE @ONLY
    )

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/VibeConfigVersion.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/Vibe${_module_name}ConfigVersion.cmake"
        IMMEDIATE @ONLY
    )

    set(CMAKE_MODULE_NAME)
    set(CMAKE_MODULE_DEPS)

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Vibe${_module_name}Config.cmake
        DESTINATION ${CMAKE_INSTALL_DEVELOPDIR}/cmake/Vibe${_module_name} COMPONENT Devel)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Vibe${_module_name}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_DEVELOPDIR}/cmake/Vibe${_module_name} COMPONENT Devel)
endmacro()

produce_cmake_script("Core" "Qt5Core")
produce_cmake_script("Gui" "Qt5DBus;Qt5Gui;Qt5Widgets;VibeCore")
produce_cmake_script("Widgets" "Qt5Gui;Qt5Widgets;VibeCore")
produce_cmake_script("Hardware" "Qt5DBus;Qt5Xml;Qt5Gui;VibeCore")
produce_cmake_script("Storage" "Qt5Xml;Qt5Gui;VibeHardware")
