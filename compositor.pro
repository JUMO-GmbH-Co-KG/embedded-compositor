QT       += core gui
TEMPLATE = subdirs
SUBDIRS = shellintegration \
        embedded-compositor \
        embeddedplatform \
        quickembeddedshellwindow \
        dev-tools

OTHER_FILES = protocol/*.xml dbus/*.xml .qmake.conf

shellintegration.depends += embeddedplatform
quickembeddedshellwindow.depends += embeddedplatform
dev-tools.depends += quickembeddedshellwindow

