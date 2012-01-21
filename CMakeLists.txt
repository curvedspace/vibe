project(Vibe)

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

# Set Vibe version
set(VIBE_VERSION_MAJOR 0)
set(VIBE_VERSION_MINOR 0)
set(VIBE_VERSION_RELEASE 0)
set(VIBE_VERSION_STR "${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_RELEASE}")

# Configure checks
include(ConfigureChecks.cmake)

# Include important macros
include(MacroOptionalFindPackage)
include(MacroLogFeature)
include(MacroBoolTo01)

# Generate header files
configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# Find Qt
set(QT_MIN_VERSION "4.7.0")
find_package(Qt4 ${QT_MIN_VERSION} REQUIRED)
include(${QT_USE_FILE})

# Find Automoc
find_package(Automoc4 REQUIRED)

# Find DBus session bus services directory
find_package(PkgConfig "0.22" REQUIRED)
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

# Subdirectories
add_subdirectory(cmake)
add_subdirectory(doxygen)
add_subdirectory(data)
add_subdirectory(headers)
add_subdirectory(src)
#add_subdirectory(samples)
