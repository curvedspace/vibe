project(QubeCore)

add_definitions(-DQUBE_CORE_LIB)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTXML_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/core ${CMAKE_SOURCE_DIR}/headers/QubeCore)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(QubeCoreSources
    qubecoreglobals.cpp
    mimetype.cpp
    singleton.h
 )

set(QubeCoreHeaders
    qube_global.h
    qubecoreglobals.h
    mimetype.h
    singleton.h
)

set(QubeCoreQrcs
    core.qrc
)

qt4_automoc(${QubeCoreSources})
qt4_add_resources(QubeCoreQrcFiles ${QubeCoreQrcs})

add_library(QubeCore SHARED ${QubeCoreSources} ${QubeCoreQrcFiles})

target_link_libraries(QubeCore
	LINK_INTERFACE_LIBRARIES ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY})
set_target_properties(QubeCore PROPERTIES
	VERSION ${QUBE_VERSION}
	SOVERSION 0.0.0)

install(FILES ${QubeCoreHeaders}
    DESTINATION ${QUBE_DEVELOPMENT_INCLUDE_DIR}/Qube/QubeCore COMPONENT Devel)
install(TARGETS QubeCore LIBRARY
    DESTINATION ${QUBE_SYSTEM_LIBRARY_DIR} COMPONENT Framework)
