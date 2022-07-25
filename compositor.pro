QT       += core gui
TEMPLATE = subdirs
SUBDIRS = shellintegration \
        compositor \
        embeddedplatform \
	quickembeddedshellwindow \
        testclients \

QT_FOR_CONFIG += waylandclient-private

OTHER_FILES = run.sh protocol/*.xml

shellintegration.depends += embeddedplatform
quickembeddedshellwindow.depends += embeddedplatform
testclients.depends += quickembeddedshellwindow
