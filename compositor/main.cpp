#include "embeddedshellextension.h"
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

int main(int argc, char *argv[]) {
  qputenv("QT_SCREEN_SCALE_FACTORS", "");
  qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "0");
  QGuiApplication app(argc, argv);
  const char *uri = "com.embeddedcompositor.embeddedshell";
  qmlRegisterType<EmbeddedShellExtensionQuickExtension>(uri, 1, 0,
                                                        "EmbeddedShell");
  qmlRegisterUncreatableType<EmbeddedShellSurfaceView>(uri, 1, 0, "SurfaceView",
                                                       "managed by wayland");
  QQmlApplicationEngine appEngine(QUrl("qrc:///main.qml"));
  return app.exec();
}
