QT       += core quick dbus
CONFIG += c++17
SOURCES +=  main.cpp \
    dbusclient.cpp
OTHER_FILES += main.qml
RESOURCES += qml.qrc

include(../common.pri)

HEADERS += \
    dbusclient.h
