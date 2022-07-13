QT       += core gui
TEMPLATE = subdirs
SUBDIRS = shellintegration \
    compositor \
	quickembeddedshellwindow \
	testclients/leftclient \
	testclients/rightclient \
	testclients/topclient \
	testclients/bottomclient

QT_FOR_CONFIG += waylandclient-private

OTHER_FILES = run.sh

shellintegration.depends += quickembeddedshellwindow
testclients/leftclient.depends+=quickembeddedshellwindow
testclients/rightclient.depends+=quickembeddedshellwindow
testclients/topclient.depends+=quickembeddedshellwindow
testclients/bottomclient.depends+=quickembeddedshellwindow
