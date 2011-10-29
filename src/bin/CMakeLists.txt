include_directories(${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/headers ${CMAKE_CURRENT_BINARY_DIR})

set(lshw_SOURCES lshw.cpp)
automoc4(lshw lshw_SOURCES)
add_executable(lshw ${lshw_SOURCES})
target_link_libraries(lshw QubeCore QubeHardware)
