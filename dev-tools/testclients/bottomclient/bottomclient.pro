QT += core quick dbus
CONFIG += c++17
SOURCES +=  main.cpp
OTHER_FILES += main.qml
RESOURCES += qml.qrc

include($$top_srcdir/embeddedplatform/embeddedplatform.pri)
include(../common.pri)
