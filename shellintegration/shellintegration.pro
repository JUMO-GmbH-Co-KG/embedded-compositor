QT += gui-private waylandclient-private quick
CONFIG += wayland-scanner c++17
TEMPLATE = lib
DEFINES += EMBEDDEDSHELLINTEGRATION_LIBRARY

QMAKE_USE += wayland-client

qtConfig(xkbcommon): \
    QMAKE_USE += xkbcommon


HEADERS += \
    embeddedshellintegration.h

SOURCES += \
    main.cpp \
	embeddedshellintegration.cpp

OTHER_FILES += \
    embedded-shell.json


TARGET = $$qtLibraryTarget($$TARGET)

PLUGIN_TYPE = wayland-shell-integration
PLUGIN_CLASS_NAME = EmbeddedShellIntegrationPlugin
load(qt_plugin)

unix:!macx: LIBS += -L$$OUT_PWD/../embeddedplatform/ -lembeddedplatform

INCLUDEPATH += $$PWD/../embeddedplatform
DEPENDPATH += $$PWD/../embeddedplatform
