// SPDX-License-Identifier: LGPL-3.0-only

#include <QtCore/QUrl>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine appEngine(QUrl("qrc:///main.qml"));
  return app.exec();
}
