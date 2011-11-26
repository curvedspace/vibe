add_definitions(-DVIBE_SERVICES_LIB)

include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/kits/core ${CMAKE_SOURCE_DIR}/headers/VibeCore)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SettingsXml
    ${CMAKE_SOURCE_DIR}/src/servers/settings/org.qubeos.Settings.xml)

set(Headers
    ${CMAKE_CURRENT_BINARY_DIR}/settingsinterface.h
)

set_source_files_properties(${SettingsXml}
    PROPERTIES CLASSNAME SettingsInterface)
qt4_add_dbus_interface(SettingsSources ${SettingsXml} settingsinterface)

add_library(VibeServices SHARED ${SettingsSources})

target_link_libraries(VibeServices
    LINK_INTERFACE_LIBRARIES ${QT_QTDBUS_LIBRARY})
set_target_properties(VibeServices PROPERTIES
    VERSION ${VIBE_VERSION_STR}
    SOVERSION ${VIBE_VERSION_MAJOR}.${VIBE_VERSION_MINOR}.${VIBE_VERSION_RELEASE})

install(FILES ${Headers}
    DESTINATION include/Vibe/VibeServices COMPONENT Devel)
install(TARGETS VibeServices
    LIBRARY DESTINATION lib COMPONENT Framework)
