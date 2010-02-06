project(Qube)

cmake_minimum_required(VERSION 2.8)

# Our CMake modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Make CPack available to easy generate binary packages
include(CPack)

# Set Qube version
set(QUBE_VERSION_MAJOR 0)
set(QUBE_VERSION_MINOR 0)
set(QUBE_VERSION RELEASE 0)
set(QUBE_VERSION "${QUBE_VERSION_MAJOR}.${QUBE_VERSION_MINOR}.${QUBE_VERSION_RELEASE}")

# Find Qt
set(QT_MIN_VERSION "4.6.0")

include(ConfigureChecks.cmake)

# Subdirectories
add_subdirectory(cmake)
add_subdirectory(data)
add_subdirectory(src)
add_subdirectory(samples)
