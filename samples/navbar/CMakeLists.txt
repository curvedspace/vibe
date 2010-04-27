project(NavbarExample)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/kits/core ${CMAKE_SOURCE_DIR}/headers/QubeCore)
include_directories(${CMAKE_SOURCE_DIR}/src/kits/gui ${CMAKE_SOURCE_DIR}/headers/QubeGui)

set(NavbarSources
    main.cpp
)

set(NavbarResources
    navbar.qrc
)

qt4_automoc(${NavbarSources})
qt4_add_resources(NavbarResourceFiles ${NavbarResources})

add_executable(navbar ${NavbarSources} ${NavbarResourceFiles})
target_link_libraries(navbar QubeGui)
