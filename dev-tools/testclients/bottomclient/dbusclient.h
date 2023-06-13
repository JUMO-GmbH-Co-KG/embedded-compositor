// SPDX-License-Identifier: LGPL-3.0-only

#ifndef DBUSCLIENT_H
#define DBUSCLIENT_H

#include <QObject>

#include "globaloverlay_interface.h"
#include "notifications_interface.h"
#include "screen_interface.h"
#include "taskswitcher_interface.h"

inline QDBusConnection getBus() {
#ifdef USE_SYSTEM_BUS
  return QDBusConnection::systemBus();
#else
  return QDBusConnection::sessionBus();
#endif
}

class DBusClient : public QObject
{
    Q_OBJECT

public:
    explicit DBusClient(QObject *parent = nullptr);
    ~DBusClient() override = default;

    Q_INVOKABLE void openTaskSwitcher();
    Q_INVOKABLE void closeTaskSwitcher();

    Q_INVOKABLE void showGlobalOverlay(const QString &message);
    Q_INVOKABLE void hideGlobalOverlay();
    Q_INVOKABLE uint notify(const QString &summary, const QString &body, const QStringList &actions);

    Q_INVOKABLE void setOrientation(const QString &orientation);

Q_SIGNALS:
    void notificationActionInvoked(unsigned int id, const QString &action);
    void notificationClosed(unsigned int id, unsigned int);

private:
    org::freedesktop::Notifications m_notificationsIface;
    com::embeddedcompositor::globaloverlay m_overlayIface;
    com::embeddedcompositor::screen m_screenIface;
    com::embeddedcompositor::taskswitcher m_taskSwitcherIface;
};

#endif // DBUSCLIENT_H
