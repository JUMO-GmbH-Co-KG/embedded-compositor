QT += core gui waylandcompositor waylandcompositor-private core-private gui-private quick dbus
CONFIG += c++17 wayland-scanner

use_system_bus {
    message(using system dbus)
    DEFINES+="USE_SYSTEM_BUS=1"
} else {
    message(using session dbus)
}

SOURCES +=  main.cpp \
            dbusinterface.cpp \
            embeddedshellextension.cpp \
            notificationmodel.cpp \
            sortfilterproxymodel.cpp
HEADERS +=  embeddedshellextension.h \
            dbus-selector.h \
            dbusinterface.h \
            notificationmodel.h \
            sortfilterproxymodel.h

OTHER_FILES += \
    qml/main.qml \
    qml/DefaultTaskSwitcher/TaskSwitcher.qml \
    qml/Notifications/Notifications.qml \
    qml/Notifications/Notification.qml

RESOURCES += qml.qrc

target.path = /usr/bin
INSTALLS += target

WAYLANDSERVERSOURCES += ../protocol/embedded-shell.xml
INCLUDEPATH += $$top_srcdir/embeddedplatform
