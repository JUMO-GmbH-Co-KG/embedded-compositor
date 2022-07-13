QT += core gui waylandcompositor waylandcompositor-private core-private gui-private quick
CONFIG += c++17 wayland-scanner
SOURCES +=  main.cpp \
    embeddedshellextension.cpp
HEADERS +=  embeddedshellextension.h

OTHER_FILES += main.qml

WAYLANDSERVERSOURCES += ../protocol/embedded-shell.xml
