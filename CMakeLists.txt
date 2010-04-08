project(Qube)

cmake_minimum_required(VERSION 2.8)

# Our CMake modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Make CPack available to easy generate binary packages
include(CPack)

# Add custom uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# Set Qube version
set(QUBE_VERSION_MAJOR 0)
set(QUBE_VERSION_MINOR 0)
set(QUBE_VERSION_RELEASE 0)
set(QUBE_VERSION "${QUBE_VERSION_MAJOR}.${QUBE_VERSION_MINOR}.${QUBE_VERSION_RELEASE}")

# Include important macros
include(ConfigureChecks.cmake)
include(FindQt4)
include(Qt4Macros)

# Find Qt
set(QT_MIN_VERSION "4.6.0")
find_package(Qt4 ${QT_MIN_VERSION} COMPONENTS QtCore REQUIRED)

# Subdirectories
add_subdirectory(cmake)
add_subdirectory(data)
add_subdirectory(headers)
add_subdirectory(src)
add_subdirectory(samples)
add_subdirectory(apps)
