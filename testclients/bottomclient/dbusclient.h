#ifndef DBUSCLIENT_H
#define DBUSCLIENT_H

#include <QObject>
#include <QtDBus/QDBusInterface>

class DBusClient : public QObject {
  Q_OBJECT
  QDBusInterface taskSwitcher{"com.basyskom.embeddedcompositor",
                              "/taskswitcher",
                              "com.embeddedcompositor.taskswitcher"};

public:
  explicit DBusClient(QObject *parent = nullptr);
  Q_INVOKABLE void openTaskSwitcher() { taskSwitcher.call("Open"); }
  Q_INVOKABLE void closeTaskSwitcher() { taskSwitcher.call("Close"); }
signals:
};

#endif // DBUSCLIENT_H
