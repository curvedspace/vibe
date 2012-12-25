project(Vibe)

cmake_minimum_required(VERSION 2.8.9)

# Enable automoc and include current directory
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set Vibe version
set(VIBE_VERSION_MAJOR 0)
set(VIBE_VERSION_MINOR 0)
set(VIBE_VERSION_PATCH 0)
set(VIBE_VERSION_STRING "${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_PATCH}")

# Our CMake modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Installation directories
include(HawaiiInstallDirs)

# Add custom uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# Set up packaging
set(CPACK_PACKAGE_NAME hawaii-vibe)
set(CPACK_PACKAGE_VERSION ${VIBE_VERSION_STRING})
set(CPACK_SYSTEM_NAME "generic")
set(CPACK_GENERATOR "TGZ")
set(CPACK_SET_DESTDIR FALSE)
include(CPack)

# Build flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden -Wall -std=c++0x")

# Assume release build with debug info by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "" FORCE)
endif()

# Disable debug output for release builds
if(CMAKE_BUILD_TYPE MATCHES "^[Rr]elease$")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()

# Include important macros
include(MacroOptionalFindPackage)
include(MacroLogFeature)
include(MacroBoolTo01)

# Include important features
include(GenerateExportHeader)

# Find Qt
set(QT_MIN_VERSION "5.0.0")
find_package(Qt5Core REQUIRED)
if(NOT Qt5Core_FOUND)
    message(FATAL_ERROR "Qt5Core module is required!")
endif()
find_package(Qt5Xml REQUIRED)
if(NOT Qt5Xml_FOUND)
    message(FATAL_ERROR "Qt5Xml module is required!")
endif()
find_package(Qt5Gui REQUIRED)
if(NOT Qt5Gui_FOUND)
    message(FATAL_ERROR "Qt5Gui module is required!")
endif()
find_package(Qt5Widgets REQUIRED)
if(NOT Qt5Widgets_FOUND)
    message(FATAL_ERROR "Qt5Widgets module is required!")
endif()
find_package(Qt5Qml REQUIRED)
if(NOT Qt5Qml_FOUND)
    message(FATAL_ERROR "Qt5Qml module is required!")
endif()
find_package(Qt5Quick REQUIRED)
if(NOT Qt5Quick_FOUND)
    message(FATAL_ERROR "Qt5Quick module is required!")
endif()
find_package(Qt5OpenGL REQUIRED)
if(NOT Qt5OpenGL_FOUND)
    message(FATAL_ERROR "Qt5OpenGL module is required!")
endif()
find_package(Qt5DBus REQUIRED)
if(NOT Qt5DBus_FOUND)
    message(FATAL_ERROR "Qt5DBus module is required!")
endif()
find_package(Qt5Designer REQUIRED)
if(NOT Qt5Designer_FOUND)
    message(FATAL_ERROR "Qt5Designer module is required!")
endif()
macro_log_feature(Qt5Core_FOUND "Qt5Core" "Support for Qt5Core" "http://qt-project.org" "")
macro_log_feature(Qt5Xml_FOUND "Qt5Xml" "Support for Qt5Xml" "http://qt-project.org" "")
macro_log_feature(Qt5Gui_FOUND "Qt5Gui" "Support for Qt5Gui" "http://qt-project.org" "")
macro_log_feature(Qt5Widgets_FOUND "Qt5Widgets" "Support for Qt5Widgets" "http://qt-project.org" "")
macro_log_feature(Qt5Qml_FOUND "Qt5Qml" "Support for Qt5Qml" "http://qt-project.org" "")
macro_log_feature(Qt5Quick_FOUND "Qt5Quick" "Support for Qt5Quick" "http://qt-project.org" "")
macro_log_feature(Qt5OpenGL_FOUND "Qt5OpenGL" "Support for Qt5OpenGL" "http://qt-project.org" "")
macro_log_feature(Qt5Designer_FOUND "Qt5Designer" "Support for Qt5Designer" "http://qt-project.org" "")

# Find Solid
find_package(solid REQUIRED)
if(NOT solid_FOUND)
    message(FATAL_ERROR "Solid module is required!")
endif()
macro_log_feature(solid_FOUND "Solid" "Support for Solid" "http://solid.kde.org" "")

# Find pkg-config
find_package(PkgConfig "0.22" REQUIRED)

# Find AccountsService
pkg_check_modules(AccountsService accountsservice REQUIRED)
if(NOT AccountsService_FOUND)
    message(FATAL_ERROR "AccountsService is required!")
endif()
macro_log_feature(AccountsService_FOUND "AccountsService" "Support for the AccountsService API" "http://www.freedesktop.org/wiki/Software/AccountsService" "")

# Find DBus interfaces directory
execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=interfaces_dir dbus-1
    OUTPUT_VARIABLE DBUS_INTERFACES_DIR
    RESULT_VARIABLE _failed)
if(_failed)
    message(FATAL_ERROR "Cannot find interfaces_dir propery from DBus: ${_failed}")
endif(_failed)
string(REGEX REPLACE "[ \t\n]+" "" DBUS_INTERFACES_DIR "${DBUS_INTERFACES_DIR}")

# Find DBus session bus services directory
execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=session_bus_services_dir dbus-1
    OUTPUT_VARIABLE DBUS_SESSION_BUS_SERVICES_DIR
    RESULT_VARIABLE _failed)
if(_failed)
    message(FATAL_ERROR "Cannot find session_bus_services_dir propery from DBus: ${_failed}")
endif(_failed)
string(REGEX REPLACE "[ \t\n]+" "" DBUS_SESSION_BUS_SERVICES_DIR "${DBUS_SESSION_BUS_SERVICES_DIR}")

# Subdirectories
add_subdirectory(cmake)
add_subdirectory(data)
add_subdirectory(headers)
add_subdirectory(src)
add_subdirectory(tests)

# Display featute log
macro_display_feature_log()
