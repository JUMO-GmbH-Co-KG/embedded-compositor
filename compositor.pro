QT       += core gui
TEMPLATE = subdirs
SUBDIRS = shell \
    compositor \
    embeddedshellwindow \
	testclients/leftclient \
	testclients/rightclient \
	testclients/topclient \
	testclients/bottomclient

QT_FOR_CONFIG += waylandclient-private

OTHER_FILES = run.sh
