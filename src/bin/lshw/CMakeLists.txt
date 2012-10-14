include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${solid_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/src/kits/core
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(${Qt5Core_DEFINITIONS})

set(lshw_SOURCES lshw.cpp)
add_executable(lshw ${lshw_SOURCES})
target_link_libraries(lshw VibeCore -L${solid_LIBRARY_DIR} ${solid_LIBRARY})
set_target_properties(lshw PROPERTIES COMPILE_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
install(TARGETS lshw DESTINATION ${CMAKE_INSTALL_BINDIR})
