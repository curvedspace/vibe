project(NavbarExample)

set(QT_USE_QTGUI, TRUE)
include(${QT_USE_FILE})

set(NavBarSources main.cpp)
qt4_automoc(${NavbarSources})

