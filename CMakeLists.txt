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

# Configure checks
include(ConfigureChecks.cmake)

# Include important macros
include(MacroOptionalFindPackage)
include(MacroLogFeature)

# Generate header files
configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# Find Qt
set(QT_MIN_VERSION "4.7.0")
find_package(Qt4 ${QT_MIN_VERSION} COMPONENTS QtCore QtXml QtDBus QtGui REQUIRED)

# Find DBus session bus services directory
find_package(PkgConfig "0.22" REQUIRED)
execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=session_bus_services_dir dbus-1
    OUTPUT_VARIABLE DBUS_SESSION_BUS_SERVICES_DIR
    RESULT_VARIABLE _failed)
if (_failed)
    message(FATAL_ERROR "Cannot find session_bus_services_dir propery from DBus: ${_failed}")
endif (_failed)
string(REGEX REPLACE "[ \t\n]+" \; DBUS_SESSION_BUS_SERVICES_DIR "${DBUS_SESSION_BUS_SERVICES_DIR}")

# Set paths variables
#set(QUBE_USERS_DIR "${CMAKE_PREFIX}/home")
#set(QUBE_APPLICATIONS_DIR "${CMAKE_PREFIX}/apps")
#set(QUBE_DATA_DIR "${CMAKE_PREFIX}/data")
#set(QUBE_DATA_FONTS_DIR "${CMAKE_PREFIX}/data/fonts")
#set(QUBE_DEVELOPMENT_DIR "${CMAKE_PREFIX}/develop")
#set(QUBE_DEVELOPMENT_INCLUDE_DIR "${CMAKE_PREFIX}/develop/include")
#set(QUBE_DEVELOPMENT_LIB_DIR "${CMAKE_PREFIX}/develop/lib")
#set(QUBE_DEVELOPMENT_CMAKE_DIR "${CMAKE_PREFIX}/develop/cmake")
#set(QUBE_DEVELOPMENT_PKGCONFIG_DIR "${CMAKE_PREFIX}/develop/pkgconfig")
#set(QUBE_SETTINGS_DIR "${CMAKE_PREFIX}/settings")
#set(QUBE_COMMON_BINARY_DIR "${CMAKE_PREFIX}/common/bin")
#set(QUBE_COMMON_LIBRARY_DIR "${CMAKE_PREFIX}/common/lib")
#set(QUBE_COMMON_PLUGINS_DIR "${CMAKE_PREFIX}/common/plugins")
#set(QUBE_COMMON_SERVERS_DIR "${CMAKE_PREFIX}/common/servers")
#set(QUBE_SYSTEM_BINARY_DIR "${CMAKE_PREFIX}/system/bin")
#set(QUBE_SYSTEM_LIBRARY_DIR "${CMAKE_PREFIX}/system/lib")
#set(QUBE_SYSTEM_PLUGINS_DIR "${CMAKE_PREFIX}/system/plugins")
#set(QUBE_SYSTEM_SERVERS_DIR "${CMAKE_PREFIX}/system/servers")

# Subdirectories
add_subdirectory(cmake)
add_subdirectory(data)
add_subdirectory(headers)
add_subdirectory(src)
add_subdirectory(samples)
