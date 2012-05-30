project(Vibe)

cmake_minimum_required(VERSION 2.8.7)

# Enable automoc
set(CMAKE_AUTOMOC ON)

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

# Set Vibe version
set(VIBE_VERSION_MAJOR 0)
set(VIBE_VERSION_MINOR 0)
set(VIBE_VERSION_PATCH 0)
set(VIBE_VERSION_STRING "${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_PATCH}")

# Build flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden -Wall -std=c++0x")

# Add -fPIC on x86_64
if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fPIC")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -fPIC")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fPIC")
endif()

# Assume release build with debug info by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "" FORCE)
endif()

# Disable debug output for release builds
if(CMAKE_BUILD_TYPE MATCHES "^[Rr]elease$")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()

# Configure checks
include(ConfigureChecks.cmake)

# Include important macros
include(MacroOptionalFindPackage)
include(MacroLogFeature)
include(MacroBoolTo01)

# Generate header files
configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# Find pkg-config
find_package(PkgConfig "0.22" REQUIRED)

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

# Find DBus session bus services directory
execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=session_bus_services_dir dbus-1
    OUTPUT_VARIABLE DBUS_SESSION_BUS_SERVICES_DIR
    RESULT_VARIABLE _failed)
if (_failed)
    message(FATAL_ERROR "Cannot find session_bus_services_dir propery from DBus: ${_failed}")
endif (_failed)
string(REGEX REPLACE "[ \t\n]+" \; DBUS_SESSION_BUS_SERVICES_DIR "${DBUS_SESSION_BUS_SERVICES_DIR}")

# Installation directories.
set(CMAKE_INSTALL_SYSTEMDIR "/system")
set(CMAKE_INSTALL_DEVELOPDIR "${CMAKE_INSTALL_SYSTEMDIR}/develop")
set(CMAKE_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_DEVELOPDIR}/headers")
set(CMAKE_INSTALL_BINDIR "${CMAKE_INSTALL_SYSTEMDIR}/bin")
set(CMAKE_INSTALL_LIBDIR "${CMAKE_INSTALL_SYSTEMDIR}/lib")
set(CMAKE_INSTALL_DATADIR "${CMAKE_INSTALL_SYSTEMDIR}/data")
set(CMAKE_INSTALL_APPSDIR "${CMAKE_INSTALL_SYSTEMDIR}/apps")
set(CMAKE_INSTALL_PLUGINSDIR "${CMAKE_INSTALL_SYSTEMDIR}/plugins")
set(CMAKE_INSTALL_IMPORTSDIR "${CMAKE_INSTALL_SYSTEMDIR}/imports")

# Subdirectories
add_subdirectory(cmake)
add_subdirectory(data)
add_subdirectory(headers)
add_subdirectory(src)
add_subdirectory(docs)
