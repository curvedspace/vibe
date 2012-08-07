add_definitions(
    ${Qt5Core_DEFINITIONS}
)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/headers/VibeCore
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_BINARY_DIR}/src/core
)

add_executable(settings settings.cpp)
set_target_properties(settings PROPERTIES COMPILE_FLAGS ${Qt5Core_EXECUTABLE_COMPILE_FLAGS})
target_link_libraries(settings VibeCore)
