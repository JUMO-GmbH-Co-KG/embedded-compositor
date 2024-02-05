QT       += core gui
TEMPLATE = subdirs
SUBDIRS = shellintegration \
        embedded-compositor \
        embeddedplatform \
        quickembeddedshellwindow \
        testclients \

testclients.subdir = dev-tools/testclients

OTHER_FILES = protocol/*.xml dbus/*.xml .qmake.conf

shellintegration.depends += embeddedplatform
quickembeddedshellwindow.depends += embeddedplatform
testclients.depends += quickembeddedshellwindow embeddedplatform
