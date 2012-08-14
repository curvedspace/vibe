add_definitions(
    ${Qt5Widgets_DEFINITIONS}
)

include_directories(
    ${Qt5Widgets_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_BINARY_DIR}/src/core
)

add_executable(vcategorizedview vcategorizedview.cpp)
set_target_properties(vcategorizedview PROPERTIES COMPILE_FLAGS ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS})
target_link_libraries(vcategorizedview VibeWidgets)
