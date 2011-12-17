add_definitions(-DVIBE_BUILDING)
add_definitions(${QT_DEFINITIONS})

include_directories(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/paged
    ${CMAKE_CURRENT_SOURCE_DIR}/preferences
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

    widgets/vtitlewidget.cpp

    paged/vpagemodel.cpp
    paged/vpageview.cpp
    paged/vpageview_p.cpp
    paged/vpagewidget.cpp
    paged/vpagewidgetmodel.cpp

    preferences/vpreferencesmodule.cpp
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
    vstyle.h
    vstyleoptionbreadcrumbbar.h
    vtheme.h

    widgets/vtitlewidget.h

    paged/vpagemodel.h
    paged/vpageview.h
    paged/vpagewidget.h
    paged/vpagewidgetmodel.h

    preferences/vpreferencesmodule.h
    preferences/vpreferencesmoduleplugin.h
)

automoc4(VibeGui SOURCES)

add_library(VibeGui SHARED ${SOURCES})
target_link_libraries(VibeGui ${QT_QTGUI_LIBRARIES} VibeCore)
set_target_properties(VibeGui PROPERTIES
    VERSION ${VIBE_VERSION_STR}
    SOVERSION ${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_RELEASE})

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeGui COMPONENT Devel)
install(TARGETS VibeGui LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Framework)
