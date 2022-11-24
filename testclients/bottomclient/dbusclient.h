#ifndef DBUSCLIENT_H
#define DBUSCLIENT_H

#include <QDebug>
#include <QObject>
#include <QtDBus/QDBusInterface>

class DBusClient : public QObject {
  Q_OBJECT
  QDBusInterface taskSwitcher{"com.basyskom.embeddedcompositor",
                              "/taskswitcher",
                              "com.embeddedcompositor.taskswitcher"};

  QDBusInterface globalOverlay{"com.basyskom.embeddedcompositor",
                               "/globaloverlay",
                               "com.embeddedcompositor.globaloverlay"};

  QDBusInterface compositorScreen{"com.basyskom.embeddedcompositor", "/screen",
                                  "com.embeddedcompositor.screen"};

  QDBusInterface notifications{"org.freedesktop.Notifications",
                               "/org/freedesktop/Notifications",
                               "org.freedesktop.Notifications"};

public:
  explicit DBusClient(QObject *parent = nullptr);
  Q_INVOKABLE void openTaskSwitcher() { taskSwitcher.call("Open"); }
  Q_INVOKABLE void closeTaskSwitcher() { taskSwitcher.call("Close"); }
  Q_INVOKABLE void showGlobalOverlay(QString message) {
    globalOverlay.call("Show", message);
  }
  Q_INVOKABLE void hideGlobalOverlay() { globalOverlay.call("Hide"); }
  Q_INVOKABLE uint notify(QString summary, QString body, QStringList actions);

  Q_INVOKABLE void setOrientation(QString orientation) {
    compositorScreen.setProperty("orientation", orientation);
  }
signals:
  void notificationActionInvoked(unsigned int id, QString action);
  void notificationClosed(unsigned int id, unsigned int);
};

#endif // DBUSCLIENT_H
