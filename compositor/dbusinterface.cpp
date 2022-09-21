#include "dbusinterface.h"
#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>

void TaskSwitcherInterface::componentComplete() {
  qDebug() << __PRETTY_FUNCTION__;
  m_valid = QDBusConnection::sessionBus().registerObject(
      "/taskswitcher", "com.embeddedcompositor.taskswitcher", this,
      QDBusConnection::ExportScriptableSlots);
  emit validChanged();
}

bool InitDbusConnection(QString serviceName) {
  if (!QDBusConnection::sessionBus().isConnected()) {
    fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
                    "To start it, run:\n"
                    "\teval `dbus-launch --auto-syntax`\n");
    return false;
  }
  if (!QDBusConnection::sessionBus().registerService(serviceName)) {
    fprintf(stderr, "%s\n",
            qPrintable(QDBusConnection::sessionBus().lastError().message()));
    return false;
  }
  return true;
}

