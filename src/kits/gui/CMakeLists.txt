add_definitions(-DVIBE_BUILDING)
add_definitions(${QT_DEFINITIONS})

include_directories(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/private
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
    dir_helper.cpp
    vstyleoptionbreadcrumbbar.cpp
    vtheme.cpp

    widgets/vlineedit.cpp
    widgets/vnavigationbar.cpp
    widgets/vtitlewidget.cpp
    widgets/private/iconbutton.cpp
    widgets/private/navigationitemdelegate.cpp

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
    vstyleoptionbreadcrumbbar.h
    vtheme.h
    vstyle.h

    widgets/vlineedit.h
    widgets/vnavigationbar.h
    widgets/vtitlewidget.h

    paged/vpagemodel.h
    paged/vpageview.h
    paged/vpagewidget.h
    paged/vpagewidgetmodel.h

    preferences/vpreferencesmodule.h
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
