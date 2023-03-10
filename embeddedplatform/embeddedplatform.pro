QT += gui core gui-private waylandclient-private quick
CONFIG += wayland-scanner c++17
TEMPLATE = lib
DEFINES += EMBEDDEDPLATFORM_LIBRARY

SOURCES += \
    embeddedplatform.cpp \
    embeddedshell.cpp \
    embeddedshellsurface.cpp

HEADERS += \
    embeddedplatform.h \
    embeddedshell.h \
    embeddedshellanchor.h \
    embeddedshellsurface.h \
    embeddedshellsurface_p.h

QMAKE_USE += wayland-client
WAYLANDCLIENTSOURCES += ../protocol/embedded-shell.xml

target.path = /usr/lib
INSTALLS += target

DISTFILES += \
    embeddedplatform.pri
