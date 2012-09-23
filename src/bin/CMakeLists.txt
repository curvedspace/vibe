include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Solid_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/src/kits/core
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(${Qt5Core_CFLAGS})

set(lshw_SOURCES lshw.cpp)
add_executable(lshw ${lshw_SOURCES})
target_link_libraries(lshw VibeCore ${Solid_LIBRARIES})
install(TARGETS lshw DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
