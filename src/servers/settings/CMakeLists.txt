project(SettingsServer)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/kits/core ${CMAKE_SOURCE_DIR}/headers/QubeCore)
include_directories(${CMAKE_SOURCE_DIR}/src/kits/gui ${CMAKE_SOURCE_DIR}/headers/QubeGui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(Sources
    main.cpp
    settingsserver.cpp
)

set(Headers
    settingsserver.h
)

qt4_automoc(${Sources})
qt4_add_dbus_adaptor(AdaptorSources org.qubeos.Settings.xml
    settingsserver.h SettingsServer)

add_executable(settings-server ${Sources} ${AdaptorSources})
add_dependencies(settings-server QubeServices)
target_link_libraries(settings-server
    ${QT_QTGUI_LIBRARY} ${QT_QTDBUS_LIBRARY})

install(TARGETS settings-server
    DESTINATION ${QUBE_SYSTEM_SERVERS_DIR} COMPONENT Servers)
install(FILES org.qubeos.Settings.xml
    DESTINATION ${DBUS_SESSION_BUS_SERVICES_DIR} COMPONENT Services)
