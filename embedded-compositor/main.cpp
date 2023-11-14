// SPDX-License-Identifier: GPL-3.0-only

#include "configurationhive.h"
#include "dbusinterface.h"
#include "embeddedshellextension.h"
#include "notificationmodel.h"
#include <QDBusMetaType>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <sortfilterproxymodel.h>

int main(int argc, char *argv[]) {
  qputenv("QT_SCREEN_SCALE_FACTORS", "");
  qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "0");

  QGuiApplication app(argc, argv);

  qmlRegisterType<EmbeddedShellExtensionQuickExtension>(
      "de.EmbeddedCompositor.embeddedshell", 1, 0, "EmbeddedShell");
  qmlRegisterUncreatableType<EmbeddedShellSurfaceView>(
      "de.EmbeddedCompositor.embeddedshell", 1, 0, "SurfaceView",
      "managed by wayland");

  qmlRegisterUncreatableType<QAbstractListModel>(
      "de.EmbeddedCompositor.embeddedshell", 1, 0, "QAbstractListModel",
      "only a property");

  qmlRegisterType<TaskSwitcherInterface>("de.EmbeddedCompositor.dbus", 1, 0,
                                         "TaskSwitcherInterface");
  qDBusRegisterMetaType<TaskSwitcherEntry>();
  qDBusRegisterMetaType<QList<TaskSwitcherEntry>>();
  qRegisterMetaType<TaskSwitcherEntry>("TaskSwitcherEntry");
  qRegisterMetaType<QList<TaskSwitcherEntry>>("QList<TaskSwitcherEntry>");

  qmlRegisterType<GlobalOverlayInterface>("de.EmbeddedCompositor.dbus", 1, 0,
                                          "GlobalOverlayInterface");

  qmlRegisterType<CompositorScreenInterface>("de.EmbeddedCompositor.dbus", 1,
                                             0, "CompositorScreenInterface");
  qmlRegisterType<NotificationModel>("de.EmbeddedCompositor.dbus", 1, 0,
                                     "NotificationModel");

  qmlRegisterType<ConfigurationHive>("de.EmbeddedCompositor.configuration", 1,
                                     0, "ConfigurationHive");

  qmlRegisterUncreatableMetaObject(EmbeddedShellTypes::staticMetaObject,
                                   "de.EmbeddedCompositor.embeddedshell", 1, 0,
                                   "EmbeddedShellTypes", "Not Instantiable!");

  if (!InitDbusConnection("de.jumo.EmbeddedCompositor")) {
    qWarning() << "failed to init dbus";
    return 1;
  }

  qmlRegisterType<SortFilterProxyModel>("de.EmbeddedCompositor.utility", 1, 0,
                                        "SortFilterProxyModel");

  QQmlApplicationEngine appEngine;
  bool exitOnQmlWarning = qgetenv("QML_WARNING_EXIT") == "1";
  QObject::connect(&appEngine, &QQmlApplicationEngine::warnings,
                   [=](auto &warnings) {
                     foreach (auto &error, warnings) {
                       qWarning() << "warning: " << error.toString();
                     }
                     if (exitOnQmlWarning) {
                       qWarning() << "exiting due to qml warning.";
                       exit(1);
                     }
                   });

  appEngine.load(QUrl("qrc:///qml/main.qml"));
  return app.exec();
}
