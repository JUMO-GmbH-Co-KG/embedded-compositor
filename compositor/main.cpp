// SPDX-License-Identifier: GPL-3.0-only

#include "dbusinterface.h"
#include "embeddedshellextension.h"
#include "notificationmodel.h"
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
      "com.embeddedcompositor.embeddedshell", 1, 0, "EmbeddedShell");
  qmlRegisterUncreatableType<EmbeddedShellSurfaceView>(
      "com.embeddedcompositor.embeddedshell", 1, 0, "SurfaceView",
      "managed by wayland");
  qmlRegisterType<TaskSwitcherInterface>("com.embeddedcompositor.dbus", 1, 0,
                                         "TaskSwitcherInterface");
  qmlRegisterType<GlobalOverlayInterface>("com.embeddedcompositor.dbus", 1, 0,
                                          "GlobalOverlayInterface");

  qmlRegisterType<CompositorScreenInterface>("com.embeddedcompositor.dbus", 1,
                                             0, "CompositorScreenInterface");
  qmlRegisterType<NotificationModel>("com.embeddedcompositor.dbus", 1, 0,
                                     "NotificationModel");

  qmlRegisterUncreatableMetaObject(EmbeddedShellTypes::staticMetaObject,
                                   "com.embeddedcompositor.embeddedshell", 1, 0,
                                   "EmbeddedShellTypes", "Not Instantiable!");

  if (!InitDbusConnection("com.basyskom.embeddedcompositor")) {
    qWarning() << "failed to init dbus";
    return 1;
  }

  qmlRegisterUncreatableType<NotificationData>("com.embeddedcompositor.dbus", 1,
                                               0, "NotificationData",
                                               "created from dbus data");

  qmlRegisterType<SortFilterProxyModel>("com.embeddedcompositor.utility", 1, 0,
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
