set(HEADERS
    VibeFluidNamespace
    VFluidFrameSvg
    VFluidSvg
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/VibeFluid COMPONENT Devel)
