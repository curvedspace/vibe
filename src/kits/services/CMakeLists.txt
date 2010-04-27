project(QubeServices)

add_definitions(-DQUBE_SERVICES_LIB)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/core ${CMAKE_SOURCE_DIR}/headers/QubeCore)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SettingsXml
    ${CMAKE_SOURCE_DIR}/src/servers/settings/org.qubeos.Settings.xml)

set(Headers
    ${CMAKE_CURRENT_BINARY_DIR}/settingsinterface.h
)

set_source_files_properties(${SettingsXml}
    PROPERTIES CLASSNAME SettingsInterface)
qt4_add_dbus_interface(SettingsSources ${SettingsXml} settingsinterface)

add_library(QubeServices SHARED ${SettingsSources})

target_link_libraries(QubeServices
	LINK_INTERFACE_LIBRARIES ${QT_QTDBUS_LIBRARY})
set_target_properties(QubeServices PROPERTIES
	VERSION ${QUBE_VERSION}
	SOVERSION 0.0.0)

install(FILES ${Headers}
    DESTINATION ${QUBE_DEVELOPMENT_INCLUDE_DIR}/Qube/QubeServices COMPONENT Devel)
install(TARGETS QubeServices LIBRARY
    DESTINATION ${QUBE_SYSTEM_LIBRARY_DIR} COMPONENT Framework)
