include_directories(
    ${Qt5Qml_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ${VIBE_CORE_INCLUDEDIR}
)

add_definitions(
    ${Qt5Qml_DEFINES}
    ${Qt5Quick_DEFINES}
    -DQT_PLUGIN
)

set(SOURCES
    fluidplugin.cpp
    iconimageprovider.cpp
)

qt5_add_resources(RESOURCES_RCC fluid.qrc)

add_library(VibeFluidPlugin SHARED ${SOURCES} ${RESOURCES_RCC})
target_link_libraries(VibeFluidPlugin ${Qt5Qml_LIBRARIES} ${Qt5Quick_LIBRARIES})

install(TARGETS VibeFluidPlugin LIBRARY DESTINATION ${CMAKE_INSTALL_IMPORTSDIR}/VibeFluid)
install(FILES qmldir DESTINATION ${CMAKE_INSTALL_IMPORTSDIR}/VibeFluid)
