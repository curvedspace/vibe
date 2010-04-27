project(SettingsServer)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/core ${CMAKE_SOURCE_DIR}/headers/QubeCore)
include_directories(${CMAKE_SOURCE_DIR}/src/gui ${CMAKE_SOURCE_DIR}/headers/QubeGui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(Sources
    main.cpp
    settingsserver.cpp
)

set(Headers
    settingsserver.h
)

set_source_files_properties(org.qubeos.Settings.xml
    PROPERTIES CLASSNAME SettingsInterface)

qt4_automoc(${Sources})
qt4_add_dbus_adaptor(AdaptorSources org.qubeos.Settings.xml
    settingsserver.h SettingsServer)
qt4_add_dbus_interface(ProxySources
    org.qubeos.Settings.xml settingsinterface)

add_executable(settings-server ${Sources} ${AdaptorSources} ${ProxySources})
target_link_libraries(settings-server
    ${QT_QTGUI_LIBRARY} ${QT_QTDBUS_LIBRARY})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/settingsinterface.h
    DESTINATION ${QUBE_DEVELOPMENT_INCLUDE_DIR}/Qube/QubeServices COMPONENT Devel)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/settings-server
    DESTINATION ${QUBE_SYSTEM_SERVERS_DIR} COMPONENT Servers)
install(FILES org.qubeos.Settings.xml
    DESTINATION ${DBUS_SESSION_BUS_SERVICES_DIR} COMPONENT Services)
