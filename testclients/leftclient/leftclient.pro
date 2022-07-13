QT       += core quick
CONFIG += c++17
SOURCES +=  main.cpp
OTHER_FILES += main.qml
RESOURCES += qml.qrc

unix:!macx: LIBS += -L$$OUT_PWD/../../quickembeddedshellwindow/EmbeddedShell -lquickembeddedshellwindow

INCLUDEPATH += $$PWD/../../quickembeddedshellwindow
DEPENDPATH += $$PWD/../../quickembeddedshellwindow
