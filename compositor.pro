QT       += core gui
TEMPLATE = subdirs
SUBDIRS = shellintegration \
        compositor \
    embeddedplatform \
	quickembeddedshellwindow \
        testclients \

QT_FOR_CONFIG += waylandclient-private

OTHER_FILES = run.sh protocol/*.xml

quickembeddedshellwindow.depends += shellintegration
testclients.depends += quickembeddedshellwindow
