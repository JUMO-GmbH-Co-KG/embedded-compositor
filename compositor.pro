QT       += core gui
TEMPLATE = subdirs
SUBDIRS = shellintegration \
        compositor \
        embeddedplatform \
        quickembeddedshellwindow \
        testclients \
        systemd

OTHER_FILES = run.sh protocol/*.xml .qmake.conf

shellintegration.depends += embeddedplatform
quickembeddedshellwindow.depends += embeddedplatform
testclients.depends += quickembeddedshellwindow

