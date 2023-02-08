QT       += core quick dbus
CONFIG += c++17
SOURCES +=  main.cpp \
    dbusclient.cpp
OTHER_FILES += main.qml
RESOURCES += qml.qrc

include(../common.pri)

DBUS_INTERFACES += \
    ../../dbus/com.embeddedcompositor.globaloverlay.xml \
    ../../dbus/com.embeddedcompositor.screen.xml \
    ../../dbus/com.embeddedcompositor.taskswitcher.xml \
    ../../dbus/org.freedesktop.Notifications.xml

HEADERS += \
    dbusclient.h
