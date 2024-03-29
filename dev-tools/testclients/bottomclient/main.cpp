// SPDX-License-Identifier: LGPL-3.0-only

#include "embeddedcompositordbusclient.h"
#include <QQmlContext>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtDBus/QDBusConnection>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  if (!getBus().isConnected()) {
    qWarning() << "Cannot connect to the D-Bus session bus.\n"
                  "To start it, run:\n"
                  "\teval `dbus-launch --auto-syntax`\n";
    return 1;
  }
  qmlRegisterUncreatableType<EmbeddedCompositorDBusInterface>("de.EmbeddedCompositor.dbusclient", 1,
                                         0, "DbusClient", "singleton");

  EmbeddedCompositorDBusInterface dbusClient;

  QQmlApplicationEngine appEngine;
  appEngine.rootContext()->setContextProperty("dbusClient", &dbusClient);
  appEngine.load(QUrl("qrc:///main.qml"));
  return app.exec();
}
