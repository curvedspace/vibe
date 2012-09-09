include_directories(
    ${Qt5Qml_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_BINARY_DIR}/src/core
)

add_definitions(
    -DVIBE_BUILDING
    ${Qt5Qml_DEFINITIONS}
    ${Qt5Quick_DEFINITIONS}
    -DQT_PLUGIN
)

set(SOURCES
    fluidplugin.cpp
    iconimageprovider.cpp
)

add_library(VibeFluidPlugin SHARED ${SOURCES})
target_link_libraries(VibeFluidPlugin ${Qt5Qml_LIBRARIES} ${Qt5Quick_LIBRARIES})

install(TARGETS VibeFluidPlugin LIBRARY DESTINATION ${CMAKE_INSTALL_IMPORTSDIR}/VibeFluid)
install(FILES qmldir DESTINATION ${CMAKE_INSTALL_IMPORTSDIR}/VibeFluid)
