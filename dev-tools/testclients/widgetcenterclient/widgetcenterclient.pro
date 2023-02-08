QT       += core gui widgets
CONFIG += c++11
SOURCES += \
    main.cpp \
    mainwindow.cpp
HEADERS += \
    mainwindow.h

include($$top_srcdir/embeddedplatform/embeddedplatform.pri)
include(../common.pri)

