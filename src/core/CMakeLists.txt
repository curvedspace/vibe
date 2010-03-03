project(QubeCore)

add_definitions(-DQUBE_CORE_LIB)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})

set(QubeCoreHeaders
    qube_global.h
)

install(FILES ${QubeCoreHeaders} DESTINATION ${INCLUDE_INSTALL_DIR}/QubeCore COMPONENT Devel)
