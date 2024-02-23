QT += core gui waylandcompositor waylandcompositor-private core-private gui-private quick dbus

QT += virtualkeyboard svg
CONFIG += c++17 wayland-scanner
# CONFIG += sanitizer sanitize_address

use_system_bus {
    message(using system dbus)
    DEFINES+="USE_SYSTEM_BUS=1"
} else {
    message(using session dbus)
}

SOURCES +=  main.cpp \
            configurationhive.cpp \
            dbusinterface.cpp \
            embeddedshellextension.cpp \
            notificationmodel.cpp \
            sortfilterproxymodel.cpp
HEADERS +=  embeddedshellextension.h \
            configurationhive.h \
            dbus-selector.h \
            dbusinterface.h \
            notificationmodel.h \
            sortfilterproxymodel.h

OTHER_FILES += \
    qml/main.qml \
    qml/DefaultTaskSwitcher/TaskSwitcher.qml \
    qml/Notifications/Notifications.qml \
    qml/Notifications/Notification.qml

# NOTE There appears to be no easy way to specify the parent class of qdbusxml2cpp through QMake.
globaloverlay_adaptor.files = ../dbus/de.EmbeddedCompositor.globaloverlay.xml
globaloverlay_adaptor.header_flags = -l GlobalOverlayInterface -i dbusinterface.h
globaloverlay_adaptor.source_flags = -l GlobalOverlayInterface

screen_adaptor.files = ../dbus/de.EmbeddedCompositor.screen.xml
screen_adaptor.header_flags = -l CompositorScreenInterface -i dbusinterface.h
screen_adaptor.source_flags = -l CompositorScreenInterface

taskswitcher_adaptor.files = ../dbus/de.EmbeddedCompositor.taskswitcher.xml
taskswitcher_adaptor.header_flags = -l TaskSwitcherInterface -i dbusinterface.h
taskswitcher_adaptor.source_flags = -l TaskSwitcherInterface

notifications_adaptor.files = ../dbus/org.freedesktop.Notifications.xml
notifications_adaptor.header_flags = -l NotificationModel -i notificationmodel.h
notifications_adaptor.source_flags = -l NotificationModel

DBUS_ADAPTORS += \
    globaloverlay_adaptor \
    screen_adaptor \
    taskswitcher_adaptor \
    notifications_adaptor

RESOURCES += qml.qrc

target.path = /usr/bin
INSTALLS += target

WAYLANDSERVERSOURCES += ../protocol/embedded-shell.xml
INCLUDEPATH += $$top_srcdir/embeddedplatform

EMBEDDED_COMPOSITOR_VERSION = $$system(git describe --tags --always)
DEFINES += EMBEDDED_COMPOSITOR_VERSION=\\\"$$EMBEDDED_COMPOSITOR_VERSION\\\"