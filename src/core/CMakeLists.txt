project(QubeCore)

add_definitions(-DQUBE_CORE_LIB)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})

set(QubeCoreHeaders
    qube_global.h
)

install(FILES ${QubeCoreHeaders} DESTINATION ${QT_INCLUDE_DIR}/Qube/QubeCore COMPONENT Devel)
