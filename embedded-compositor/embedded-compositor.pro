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
            embeddedshellextension.cpp \
            sortfilterproxymodel.cpp

HEADERS +=  embeddedshellextension.h \
            configurationhive.h \
            dbus-selector.h \
            sortfilterproxymodel.h

OTHER_FILES += \
    qml/main.qml \
    qml/DefaultTaskSwitcher/TaskSwitcher.qml \
    qml/Notifications/Notifications.qml \

# NOTE There appears to be no easy way to specify the parent class of qdbusxml2cpp through QMake.
globaloverlay_adaptor.files = ../dbus/de.EmbeddedCompositor.globaloverlay.xml
globaloverlay_adaptor.header_flags = -l GlobalOverlayDBusInterface -i ./dbus/GlobalOverlayDBusInterface.hpp
globaloverlay_adaptor.source_flags = -l GlobalOverlayDBusInterface

screen_adaptor.files = ../dbus/de.EmbeddedCompositor.screen.xml
screen_adaptor.header_flags = -l CompositorScreenDBusInterface -i ./dbus/CompositorScreenDBusInterface.hpp
screen_adaptor.source_flags = -l CompositorScreenDBusInterface

taskswitcher_adaptor.files = ../dbus/de.EmbeddedCompositor.taskswitcher.xml
taskswitcher_adaptor.header_flags = -l TaskSwitcherDBusInterface -i ./dbus/TaskSwitcherDBusInterface.hpp
taskswitcher_adaptor.source_flags = -l TaskSwitcherDBusInterface

notifications_adaptor.files = ../dbus/org.freedesktop.Notifications.xml
notifications_adaptor.header_flags = -l NotificationModel -i ./dbus/NotificationModel.hpp
notifications_adaptor.source_flags = -l NotificationModel

screenshot_adaptor.files = ../dbus/de.EmbeddedCompositor.screenshot.xml
screenshot_adaptor.header_flags = -l ScreenShotDBusInterface -i ./dbus/ScreenShotDBusInterface.hpp
screenshot_adaptor.source_flags = -l ScreenShotDBusInterface

DBUS_ADAPTORS += \
    globaloverlay_adaptor \
    screen_adaptor \
    taskswitcher_adaptor \
    notifications_adaptor \
    screenshot_adaptor

RESOURCES += qml.qrc

target.path = /usr/bin
INSTALLS += target

WAYLANDSERVERSOURCES += ../protocol/embedded-shell.xml
INCLUDEPATH += $$top_srcdir/embeddedplatform

EMBEDDED_COMPOSITOR_VERSION = $$system(git describe --tags --always)
DEFINES += EMBEDDED_COMPOSITOR_VERSION=\\\"$$EMBEDDED_COMPOSITOR_VERSION\\\"

include(./dbus/dbus-interfaces.pri)
