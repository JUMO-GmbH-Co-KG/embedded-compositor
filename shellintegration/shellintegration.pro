QT += gui-private waylandclient-private quick
CONFIG += c++17 plugin relative_qt_rpath
TEMPLATE = lib
DEFINES += EMBEDDEDSHELLINTEGRATION_LIBRARY

HEADERS += \
    embeddedshellintegration.h

SOURCES += \
    main.cpp \
  	embeddedshellintegration.cpp

OTHER_FILES += \
    embedded-shell.json

target = $$qtLibraryTarget($$TARGET)
target.path=/usr/lib/plugins/wayland-shell-integration

PLUGIN_TYPE = wayland-shell-integration
PLUGIN_CLASS_NAME = EmbeddedShellIntegrationPlugin
include($$top_srcdir/embeddedplatform/embeddedplatform.pri)

INSTALLS += target
