QT -= gui
QT += quick

TEMPLATE = lib
DEFINES += QUICKEMBEDDEDSHELLWINDOW_LIBRARY

CONFIG += c++17

SOURCES += \
    quickembeddedshellwindow.cpp

HEADERS += \
    quickembeddedshell.h \
    quickembeddedshellwindow_global.h \
    quickembeddedshellwindow.h
OTHER_FILES = qmldir

TARGET = $$qtLibraryTarget($$TARGET)

uri = EmbeddedShell
DESTDIR = $$uri



DISTFILES += qmldir

#Copies the qmldir file to the build directory
!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/$$uri/qmldir
	copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
	copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
	QMAKE_EXTRA_TARGETS += copy_qmldir
	PRE_TARGETDEPS += $$copy_qmldir.target
}

#Copies the qmldir file and the built plugin .so to the QT_INSTALL_QML directory
qmldir.files = qmldir
installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
qmldir.path = $$installPath
target.path = $$installPath
INSTALLS += target qmldir
