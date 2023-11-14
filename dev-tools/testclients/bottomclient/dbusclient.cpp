// SPDX-License-Identifier: LGPL-3.0-only

#include "dbusclient.h"

#include <QDBusConnection>

static const QString compositorServiceName()
{
    return QStringLiteral("de.jumo.EmbeddedCompositor");
}

DBusClient::DBusClient(QObject *parent)
    : QObject(parent)
    , m_notificationsIface(QStringLiteral("org.freedesktop.Notifications"), QStringLiteral("/org/freedesktop/Notifications"), getBus())
    , m_overlayIface(compositorServiceName(), QStringLiteral("/globaloverlay"), getBus())
    , m_screenIface(compositorServiceName(), QStringLiteral("/screen"), getBus())
    , m_taskSwitcherIface(compositorServiceName(), QStringLiteral("/taskswitcher"), getBus())
{
    connect(&m_notificationsIface, &org::freedesktop::Notifications::ActionInvoked, this, &DBusClient::notificationActionInvoked);
    connect(&m_notificationsIface, &org::freedesktop::Notifications::NotificationClosed, this, &DBusClient::notificationClosed);
}

void DBusClient::openTaskSwitcher()
{
    m_taskSwitcherIface.Open();
}

void DBusClient::closeTaskSwitcher()
{
    m_taskSwitcherIface.Close();
}

void DBusClient::showGlobalOverlay(const QString &message)
{
    m_overlayIface.Show(message);
}

void DBusClient::hideGlobalOverlay()
{
    m_overlayIface.Hide();
}

uint DBusClient::notify(const QString &summary, const QString &body, const QStringList &actions)
{
    QStringList actionList;
    actionList.reserve(actions.count() * 2);
    // Notification actions are pairs of name - label.
    // For now, use the label as action name.
    for (const QString &action : actions) {
        actionList << action << action;
    }

    const auto reply = m_notificationsIface.Notify(
        QStringLiteral("bottom client"), // app_name
        0, // replaces_id
        QStringLiteral("no-icon"), // app_icon
        summary,
        body,
        actionList,
        QVariantMap{}, // hints
        -1 // timeout
    );
    return reply.value();
}

void DBusClient::setOrientation(const QString &orientation)
{
    m_screenIface.setOrientation(orientation);
}
