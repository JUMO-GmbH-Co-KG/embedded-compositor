#include <QtCore/QUrl>
#include <QtCore/QDebug>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    qputenv("QT_IVI_SURFACE_ID","1000");
    qputenv("QT_WAYLAND_SHELL_INTEGRATION","ivi-shell");
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine appEngine(QUrl("qrc:///main.qml"));
    return app.exec();
}
