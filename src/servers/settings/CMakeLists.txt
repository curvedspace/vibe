include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/kits/core ${CMAKE_SOURCE_DIR}/headers/QubeCore)
include_directories(${CMAKE_SOURCE_DIR}/src/kits/gui ${CMAKE_SOURCE_DIR}/headers/QubeGui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    main.cpp
    settingsserver.cpp
)

set(HEADERS
    settingsserver.h
)

qt4_automoc(${SOURCES})
qt4_add_dbus_adaptor(ADAPTOR_SOURCES org.qubeos.Settings.xml
    settingsserver.h SettingsServer)

add_executable(settings-server ${SOURCES} ${ADAPTOR_SOURCES})
add_dependencies(settings-server QubeServices)
target_link_libraries(settings-server
    ${QT_QTGUI_LIBRARY} ${QT_QTDBUS_LIBRARY})

install(TARGETS settings-server
    RUNTIME DESTINATION bin COMPONENT Servers)
install(FILES org.qubeos.Settings.xml
    DESTINATION ${DBUS_SESSION_BUS_SERVICES_DIR} COMPONENT Services)
