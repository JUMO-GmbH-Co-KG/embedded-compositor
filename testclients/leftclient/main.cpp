#include <QtCore/QUrl>
#include <QtCore/QDebug>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include "embeddedshellwindow.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine appEngine(QUrl("qrc:///main.qml"));
    QuickEmbeddedShellWindow::registerQml();
    return app.exec();
}
