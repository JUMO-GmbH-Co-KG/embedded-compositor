#include <QtCore/QUrl>
#include <QtCore/QDebug>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    qputenv("QT_SCREEN_SCALE_FACTORS","");
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR","0");
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine appEngine(QUrl("compositor/main.qml"));
    return app.exec();
}
