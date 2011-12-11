add_definitions(${QT_DEFINITIONS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

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

qt4_wrap_ui(FORMS_HEADERS ${FORMS})

automoc4(galley SOURCES)
add_executable(gallery ${SOURCES} ${FORMS_HEADERS})
target_link_libraries(gallery ${QT_LIBRARIES})
