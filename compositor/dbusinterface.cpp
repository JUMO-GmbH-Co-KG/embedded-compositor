// SPDX-License-Identifier: GPL-3.0-only

#include "dbusinterface.h"
#include "dbus-selector.h"
#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>

void TaskSwitcherInterface::componentComplete() {
  m_valid = getBus().registerObject("/taskswitcher", this,
                                    QDBusConnection::ExportScriptableSlots);
  emit validChanged();
}

bool InitDbusConnection(QString serviceName) {
  auto bus = getBus();
  if (!bus.isConnected()) {
    qWarning() << "Cannot connect to the D-Bus.\n"
                  "To start it, run:\n"
                  "\teval `dbus-launch --auto-syntax`\n";
    return false;
  }
  if (!bus.registerService(serviceName)) {
    qDebug() << bus.lastError().message();
    return false;
  }
  return true;
}

void GlobalOverlayInterface::componentComplete() {
  m_valid = getBus().registerObject("/globaloverlay", this,
                                    QDBusConnection::ExportScriptableSlots);
  emit validChanged();
}

CompositorScreenInterface::CompositorScreenInterface() {
  auto bootScreenOrientation = qgetenv("SCREEN_ORIENTATION");
  if (!bootScreenOrientation.isNull())
    m_orientation = QString(bootScreenOrientation);
}

void CompositorScreenInterface::componentComplete() {
  m_valid = getBus().registerObject("/screen", this,
                                    QDBusConnection::ExportAllProperties);
  emit validChanged();
}

const QString &CompositorScreenInterface::orientation() const {
  return m_orientation;
}

void CompositorScreenInterface::setOrientation(const QString &newOrientation) {
  if (m_orientation == newOrientation)
    return;
  m_orientation = newOrientation;
  emit orientationChanged(m_orientation);
}
