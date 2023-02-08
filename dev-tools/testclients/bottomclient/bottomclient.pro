QT       += core quick dbus
CONFIG += c++17
SOURCES +=  main.cpp \
    dbusclient.cpp
OTHER_FILES += main.qml
RESOURCES += qml.qrc

include(../common.pri)

DBUS_INTERFACES += \
    $$top_srcdir/dbus/com.embeddedcompositor.globaloverlay.xml \
    $$top_srcdir/dbus/com.embeddedcompositor.screen.xml \
    $$top_srcdir/dbus/com.embeddedcompositor.taskswitcher.xml \
    $$top_srcdir/dbus/org.freedesktop.Notifications.xml

HEADERS += \
    dbusclient.h
