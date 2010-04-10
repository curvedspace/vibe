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

# Find Qt
set(QT_MIN_VERSION "4.6.0")
find_package(Qt4 ${QT_MIN_VERSION} COMPONENTS QtCore QtGui REQUIRED)

# Set paths variables
set(QUBE_USERS_DIR "${CMAKE_PREFIX}/Users")
set(QUBE_APPLICATIONS_DIR "${CMAKE_PREFIX}/Applications")
set(QUBE_DATA_DIR "${CMAKE_PREFIX}/Data")
set(QUBE_DATA_FONTS_DIR "${CMAKE_PREFIX}/Data/Fonts")
set(QUBE_DEVELOPMENT_DIR "${CMAKE_PREFIX}/Development")
set(QUBE_DEVELOPMENT_INCLUDE_DIR "${CMAKE_PREFIX}/Development/Include")
set(QUBE_DEVELOPMENT_CMAKE_DIR "${CMAKE_PREFIX}/Development/CMake")
set(QUBE_DEVELOPMENT_PKGCONFIG_DIR "${CMAKE_PREFIX}/Development/PkgConfig")
set(QUBE_LIBRARY_DIR "${CMAKE_PREFIX}/Library")
set(QUBE_PLUGIN_DIR "${CMAKE_PREFIX}/Plugin")
set(QUBE_ETC_DIR "${CMAKE_PREFIX}/etc")
set(QUBE_BIN_DIR "${CMAKE_PREFIX}/bin")
set(QUBE_LIB_DIR "${CMAKE_PREFIX}/lib")

# Subdirectories
add_subdirectory(cmake)
add_subdirectory(data)
add_subdirectory(headers)
add_subdirectory(src)
add_subdirectory(samples)
