add_definitions(-DVIBE_BUILDING)

include_directories(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
    vaboutdialog.cpp
    vabstractbreadcrumbmodel.cpp
    vbreadcrumbbar.cpp
    vbreadcrumbdirmodel.cpp
    vcolorscheme.cpp
    vclickablelabel.cpp
    dir_helper.cpp
    vnavbar.cpp
    vnavbargroup.cpp
    vnavbaritem.cpp
    vselectablewidget.cpp
    vstyleoptionbreadcrumbbar.cpp
    vtheme.cpp
)

set(HEADERS
    vaboutdialog.h
    vabstractbreadcrumbmodel.h
    vbreadcrumbbar.h
    vbreadcrumbdirmodel.h
    vcolorscheme.h
    vclickablelabel.h
    vinterfacemacros.h
    vnavbar.h
    vnavbargroup.h
    vnavbaritem.h
    vselectablewidget.h
    vstyleoptionbreadcrumbbar.h
    vtheme.h
)

automoc4(VibeGui SOURCES)

add_library(VibeGui SHARED ${SOURCES})
target_link_libraries(VibeGui
    LINK_INTERFACE_LIBRARIES ${QT_QTGUI_LIBRARY} VibeCore)
set_target_properties(VibeGui PROPERTIES
    VERSION ${VIBE_VERSION_STR}
    SOVERSION ${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_RELEASE})

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeGui COMPONENT Devel)
install(TARGETS VibeGui LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Framework)
