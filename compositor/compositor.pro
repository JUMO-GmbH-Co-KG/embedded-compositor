QT += core gui waylandcompositor waylandcompositor-private core-private gui-private quick
CONFIG += c++17 wayland-scanner
SOURCES +=  main.cpp \
            embeddedshellextension.cpp
HEADERS +=  embeddedshellextension.h

OTHER_FILES += main.qml
RESOURCES += qml.qrc

target.path = /usr/bin
INSTALLS += target

WAYLANDSERVERSOURCES += ../protocol/embedded-shell.xml
