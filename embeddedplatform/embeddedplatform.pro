QT += gui core gui-private waylandclient-private quick dbus
CONFIG += wayland-scanner c++17

TEMPLATE = lib
DEFINES += EMBEDDEDPLATFORM_LIBRARY
SOURCES += \
    embeddedcompositordbusclient.cpp \
    embeddedplatform.cpp \
    embeddedshell.cpp \
    embeddedshellsurface.cpp

QDBUSXML2CPP_INTERFACE_HEADER_FLAGS = -i $$top_srcdir/embedded-compositor/dbusinterface.h
DBUS_INTERFACES += \
    $$top_srcdir/dbus/de.EmbeddedCompositor.globaloverlay.xml \
    $$top_srcdir/dbus/de.EmbeddedCompositor.screen.xml \
    $$top_srcdir/dbus/de.EmbeddedCompositor.taskswitcher.xml \
    $$top_srcdir/dbus/org.freedesktop.Notifications.xml

HEADERS += \
    embeddedcompositordbusclient.h \
    embeddedplatform.h \
    embeddedshell.h \
    embeddedshellanchor.h \
    embeddedshellsurface.h \
    embeddedshellsurface_p.h

QMAKE_USE += wayland-client
WAYLANDCLIENTSOURCES += ../protocol/embedded-shell.xml

headers.path = /usr/include/embedded-compositor
headers.files += \
    embeddedplatform.h \
    embeddedshellanchor.h \
    embeddedshellsurface.h

INSTALLS += headers

target.path = /usr/lib
INSTALLS += target

DISTFILES += \
    embeddedplatform.pri
