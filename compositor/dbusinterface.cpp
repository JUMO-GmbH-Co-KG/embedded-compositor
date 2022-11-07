#include "dbusinterface.h"
#include "dbus-selector.h"
#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>

void TaskSwitcherInterface::componentComplete() {
  m_valid = getBus().registerObject("/taskswitcher",
                                    "com.embeddedcompositor.taskswitcher", this,
                                    QDBusConnection::ExportScriptableSlots);
  emit validChanged();
}

bool InitDbusConnection(QString serviceName) {
  auto bus = getBus();
  if (!bus.isConnected()) {
    fprintf(stderr, "Cannot connect to the D-Bus.\n"
                    "To start it, run:\n"
                    "\teval `dbus-launch --auto-syntax`\n");
    return false;
  }
  if (!bus.registerService(serviceName)) {
    fprintf(stderr, "%s\n", qPrintable(bus.lastError().message()));
    return false;
  }
  return true;
}

void GlobalOverlayInterface::componentComplete() {
  m_valid = getBus().registerObject(
      "/globaloverlay", "com.embeddedcompositor.globaloverlay", this,
      QDBusConnection::ExportScriptableSlots);
  emit validChanged();
}
