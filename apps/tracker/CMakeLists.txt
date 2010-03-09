project(Tracker)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/core ${CMAKE_SOURCE_DIR}/headers/QubeCore)
include_directories(${CMAKE_SOURCE_DIR}/src/gui ${CMAKE_SOURCE_DIR}/headers/QubeGui)
include_directories(${CMAKE_SOURCE_DIR}/headers)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(TrackerSources
    main.cpp
)

set(TrackerMocSources
    mainwindow.cpp
)

set(TrackerUis
    mainwindow.ui
)

set(TrackerResources
    tracker.qrc
)

qt4_automoc(${TrackerMocSources})
qt4_wrap_ui(TrackerUiFiles ${TrackerUis})
qt4_add_resources(TrackerResourceFiles ${TrackerResources})

add_executable(tracker ${TrackerSources} ${TrackerMocSources} ${TrackerUiFiles} ${TrackerResourceFiles})
target_link_libraries(tracker ${QT_LIBRARIES} QubeGui)
