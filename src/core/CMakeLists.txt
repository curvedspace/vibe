project(QubeCore)

add_definitions(-DQUBE_CORE_LIB)

include_directories(${QT_INCLUDES})

install(FILES
	qube_global.h
DESTINATION ${INCLUDE_INSTALL_DIR}/QubeCore COMPONENT Devel)
