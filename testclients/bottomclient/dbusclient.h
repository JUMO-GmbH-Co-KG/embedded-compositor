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

  QDBusInterface notifications{"org.freedesktop.Notifications",
                               "/org/freedesktop/Notifications",
                               "org.freedesktop.Notifications"};

public:
  explicit DBusClient(QObject *parent = nullptr);
  Q_INVOKABLE void openTaskSwitcher() { taskSwitcher.call("Open"); }
  Q_INVOKABLE void closeTaskSwitcher() { taskSwitcher.call("Close"); }
  Q_INVOKABLE uint notify(QString summary, QString body, QStringList actions) {
    auto message = notifications.callWithArgumentList(
        QDBus::CallMode::Block, "Notify",
        {"bottom client", 0u, "no-icon", summary, body, actions, QVariantMap(),
         -1

        });
    qDebug() << message.errorMessage() << message.errorName() << message.type()
             << message.arguments();

    return message.arguments().first().toUInt();
  }
signals:
  void notificationActionInvoked(unsigned int id, QString action);
  void notificationClosed(unsigned int id, unsigned int);
private slots:
  void testNoti(unsigned int id, QString action) {
    qDebug() << __PRETTY_FUNCTION__ << id << action;
  }
};

#endif // DBUSCLIENT_H
