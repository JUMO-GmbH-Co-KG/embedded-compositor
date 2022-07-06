QT += gui-private waylandclient-private \
    quick
CONFIG += wayland-scanner c++17

QMAKE_USE += wayland-client

qtConfig(xkbcommon): \
    QMAKE_USE += xkbcommon

WAYLANDCLIENTSOURCES += ../protocol/embedded-shell.xml

HEADERS += \
    embeddedshellintegration.h \
	embeddedshellsurface.h

SOURCES += \
    main.cpp \
	embeddedshellintegration.cpp \
	embeddedshellsurface.cpp

OTHER_FILES += \
    embedded-shell.json

PLUGIN_TYPE = wayland-shell-integration
PLUGIN_CLASS_NAME = EmbeddedShellIntegrationPlugin
load(qt_plugin)

unix:!macx: LIBS += -L$$OUT_PWD/../embeddedshellwindow/ -lembeddedshellwindow

INCLUDEPATH += $$PWD/../embeddedshellwindow
DEPENDPATH += $$PWD/../embeddedshellwindow
