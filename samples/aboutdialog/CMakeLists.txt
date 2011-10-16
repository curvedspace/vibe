include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/headers)

set(SOURCES
    main.cpp
)

set(RESOURCES
    aboutdialog.qrc
)

qt4_add_resources(RESOURCES_RCC ${RESOURCES})

add_executable(aboutdialog ${SOURCES} ${RESOURCES_RCC})
target_link_libraries(aboutdialog QubeGui)
