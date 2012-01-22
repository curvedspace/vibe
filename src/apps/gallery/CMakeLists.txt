include_directories(
    ${Qt5Widgets_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
    main.cpp
    previewwidget.cpp
)

set(HEADERS
    previewwidget.h
)

set(FORMS
    previewwidget.ui
)

qt5_wrap_ui(FORMS_HEADERS ${FORMS})

add_executable(gallery ${SOURCES} ${FORMS_HEADERS})
target_link_libraries(gallery ${Qt5Widgets_LIBRARIES})
