TARGET = $$qtLibraryTarget(qnavbarplugin)
TEMPLATE = lib
CONFIG += designer \
    plugin

HEADERS = qnavbarplugin.h
SOURCES = qnavbarplugin.cpp
RESOURCES = icons.qrc

INCLUDEPATH += ../core ../gui
LIBS += -F../../build -framework QubeGui

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target
