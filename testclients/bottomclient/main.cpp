#include "dbusclient.h"
#include <QQmlContext>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtDBus/QDBusConnection>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  if (!QDBusConnection::sessionBus().isConnected()) {
    fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
                    "To start it, run:\n"
                    "\teval `dbus-launch --auto-syntax`\n");
    return 1;
  }
  qmlRegisterUncreatableType<DBusClient>("com.embeddedcompositor.dbusclient", 1,
                                         0, "DbusClient", "singleton");

  DBusClient dbusClient;

  QQmlApplicationEngine appEngine(QUrl("qrc:///main.qml"));
  appEngine.rootContext()->setContextProperty("dbusClient", &dbusClient);
  return app.exec();
}
