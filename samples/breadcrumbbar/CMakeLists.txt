include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/headers)

set(SOURCES
    main.cpp
)

add_executable(breadcrumbbar ${SOURCES} ${RESOURCES_RCC})
target_link_libraries(breadcrumbbar QubeGui)
