set(HEADERS
    VColorScheme
    VMainApplicationAdaptor
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeGui COMPONENT Devel)
