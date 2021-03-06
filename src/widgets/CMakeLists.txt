include_directories(
    ${solid_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/src/gui
    ${CMAKE_BINARY_DIR}/src/core
    ${CMAKE_BINARY_DIR}/src/gui
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
    widgets/vtitlewidget.cpp
    widgets/private/iconbutton.cpp
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
    paged/vpagemodel.h
    paged/vpageview.h
    paged/vpagewidget.h
    paged/vpagewidgetmodel.h
    preferences/vpreferencesmodule.h
    preferences/vpreferencesmoduleplugin.h
    widgets/vbreadcrumbview.h
    widgets/vcolorbutton.h
    widgets/vlineedit.h
    widgets/vtitlewidget.h
)

# Add library
add_compiler_export_flags()
add_library(VibeWidgets SHARED ${SOURCES})
generate_export_header(VibeWidgets
    BASE_NAME vibewidgets
    DEPRECATED_MACRO_NAME VIBE_DEPRECATED
)
qt5_use_modules(VibeWidgets DBus Widgets)
target_link_libraries(VibeWidgets
    VibeCore
    VibeGui
    -L${solid_LIBRARY_DIR} ${solid_LIBRARY}
)
set_target_properties(VibeWidgets PROPERTIES
    VERSION ${VIBE_VERSION_STRING}
    SOVERSION ${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_PATCH})

# Install files
set(HEADERS ${HEADERS} ${CMAKE_CURRENT_BINARY_DIR}/vibewidgets_export.h)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeWidgets)
install(TARGETS VibeWidgets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
