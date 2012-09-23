add_definitions(
    -DVIBE_BUILDING
    ${Qt5Core_DEFINITIONS}
    ${Qt5DBus_DEFINITIONS}
    ${Qt5Gui_DEFINITIONS}
    ${Qt5Widgets_DEFINITIONS}
)

set(CMAKE_CXX_FLAGS "${Qt5Widgets_LIBRARY_COMPILE_FLAGS}")

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5DBus_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${solid_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/src/gui
    ${CMAKE_BINARY_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/dialogs
    ${CMAKE_CURRENT_SOURCE_DIR}/itemviews
    ${CMAKE_CURRENT_SOURCE_DIR}/paged
    ${CMAKE_CURRENT_SOURCE_DIR}/preferences
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/private
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Sources
set(SOURCES
    dialogs/vaboutdialog.cpp
    itemviews/vcategorizedsortfilterproxymodel.cpp
    itemviews/vcategorizedview.cpp
    itemviews/vcategorydrawer.cpp
    itemviews/vfileplacesmodel.cpp
    itemviews/vfileplacesitem.cpp
    itemviews/vfileplacesviewdelegate.cpp
    itemviews/vfileplacesview.cpp
    itemviews/vfilesystemmodel.cpp
    kernel/vapplication.cpp
    paged/vpagemodel.cpp
    paged/vpageview.cpp
    paged/vpageview_p.cpp
    paged/vpagewidget.cpp
    paged/vpagewidgetmodel.cpp
    preferences/vpreferencesmodule.cpp
    preferences/vpreferencesmoduleplugin.cpp
    widgets/vbreadcrumbview.cpp
    widgets/vcolorbutton.cpp
    widgets/vlineedit.cpp
    widgets/vnavigationbar.cpp
    widgets/vtitlewidget.cpp
    widgets/private/iconbutton.cpp
    widgets/private/navigationitemdelegate.cpp
)

# Headers
set(HEADERS
    dialogs/vaboutdialog.h
    itemviews/vcategorizedsortfilterproxymodel.h
    itemviews/vcategorizedview.h
    itemviews/vcategorydrawer.h
    itemviews/vfileplacesmodel.h
    itemviews/vfileplacesview.h
    itemviews/vfilesystemmodel.h
    kernel/vapplication.h
    paged/vpagemodel.h
    paged/vpageview.h
    paged/vpagewidget.h
    paged/vpagewidgetmodel.h
    preferences/vpreferencesmodule.h
    preferences/vpreferencesmoduleplugin.h
    widgets/vbreadcrumbview.h
    widgets/vcolorbutton.h
    widgets/vlineedit.h
    widgets/vnavigationbar.h
    widgets/vtitlewidget.h
)

# Add library
add_library(VibeWidgets SHARED ${SOURCES})
target_link_libraries(VibeWidgets ${Qt5DBus_LIBRARIES} ${Qt5Widgets_LIBRARIES}
    VibeCore VibeGui
    -L${solid_LIBRARY_DIR} ${solid_LIBRARY})
set_target_properties(VibeWidgets PROPERTIES
    VERSION ${VIBE_VERSION_STRING}
    SOVERSION ${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_PATCH})

# Install files
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeWidgets)
install(TARGETS VibeWidgets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
