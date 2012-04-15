add_definitions(-DVIBE_BUILDING)

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5DBus_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/kits/core
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/private
    ${CMAKE_CURRENT_SOURCE_DIR}/paged
    ${CMAKE_CURRENT_SOURCE_DIR}/preferences
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
    vaboutdialog.cpp
    vcolorscheme.cpp
    dir_helper.cpp
    vtheme.cpp

    kernel/vmainapplicationadaptor.cpp

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
    vcolorscheme.h
    vtheme.h

    kernel/vmainapplicationadaptor.h

    widgets/vlineedit.h
    widgets/vnavigationbar.h
    widgets/vtitlewidget.h

    paged/vpagemodel.h
    paged/vpageview.h
    paged/vpagewidget.h
    paged/vpagewidgetmodel.h

    preferences/vpreferencesmodule.h
    preferences/vpreferencesmoduleplugin.h
)

add_library(VibeGui SHARED ${SOURCES})
target_link_libraries(VibeGui ${Qt5DBus_LIBRARIES} ${Qt5Gui_LIBRARIES}
    ${Qt5Widgets_LIBRARIES} VibeCore)
set_target_properties(VibeGui PROPERTIES
    VERSION ${VIBE_VERSION_STRING}
    SOVERSION ${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_RELEASE})

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeGui COMPONENT Devel)
install(TARGETS VibeGui LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Framework)
