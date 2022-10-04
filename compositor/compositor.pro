QT += core gui waylandcompositor waylandcompositor-private core-private gui-private quick dbus
CONFIG += c++17 wayland-scanner
SOURCES +=  main.cpp \
            dbusinterface.cpp \
            embeddedshellextension.cpp
HEADERS +=  embeddedshellextension.h \
            dbusinterface.h

OTHER_FILES += \
    qml/main.qml
RESOURCES += qml.qrc

target.path = /usr/bin
INSTALLS += target

WAYLANDSERVERSOURCES += ../protocol/embedded-shell.xml
INCLUDEPATH += $$top_srcdir/embeddedplatform

DISTFILES += \
    qml/DefaultTaskSwitcher/TaskSwitcher.qml
