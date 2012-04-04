include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${VibeCore_INCLUDE_DIRS}
    ${VibeGui_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(${VibeGui_CFLAGS} -DQT_PLUGIN)

set(SOURCES
    main.cpp
    vanishcolorutils.cpp
    vanishprimitives.cpp
    vanishstyle.cpp
    vanishutils.cpp
    qstylehelper.cpp
)

add_library(vanish SHARED ${SOURCES})
set_target_properties(vanish PROPERTIES PREFIX "")
target_link_libraries(vanish VibeGui)

install(TARGETS vanish LIBRARY
    DESTINATION ${CMAKE_INSTALL_PLUGINSDIR}/styles COMPONENT Framework)
