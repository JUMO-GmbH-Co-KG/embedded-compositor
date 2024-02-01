// SPDX-License-Identifier: LGPL-3.0-only

#include "embeddedcompositordbusclient.h"


#include "globaloverlay_interface.h"
#include "notifications_interface.h"
#include "screen_interface.h"
#include "taskswitcher_interface.h"


class EmbeddedCompositorDBusInterfacePrivate {
    friend class EmbeddedCompositorDBusInterface;
public:
    EmbeddedCompositorDBusInterfacePrivate()
        : m_notificationsIface(QStringLiteral("org.freedesktop.Notifications"), QStringLiteral("/org/freedesktop/Notifications"), getBus())
        , m_overlayIface(compositorServiceName(), QStringLiteral("/globaloverlay"), getBus())
        , m_screenIface(compositorServiceName(), QStringLiteral("/screen"), getBus())
        , m_taskSwitcherIface(compositorServiceName(), QStringLiteral("/taskswitcher"), getBus())
    {}

    static const QString compositorServiceName()
    {
        return QStringLiteral("de.jumo.EmbeddedCompositor");
    }
private:
    org::freedesktop::Notifications m_notificationsIface;
    de::EmbeddedCompositor::globaloverlay m_overlayIface;
    de::EmbeddedCompositor::screen m_screenIface;
    de::EmbeddedCompositor::taskswitcher m_taskSwitcherIface;
};


EmbeddedCompositorDBusInterface::EmbeddedCompositorDBusInterface(QObject *parent)
    : QObject(parent), m_d(new EmbeddedCompositorDBusInterfacePrivate)
{
    Q_D(EmbeddedCompositorDBusInterface);

    connect(&d->m_notificationsIface, &org::freedesktop::Notifications::ActionInvoked, this, &EmbeddedCompositorDBusInterface::notificationActionInvoked);
    connect(&d->m_notificationsIface, &org::freedesktop::Notifications::NotificationClosed, this, &EmbeddedCompositorDBusInterface::notificationClosed);
}

EmbeddedCompositorDBusInterface::~EmbeddedCompositorDBusInterface() = default;

void EmbeddedCompositorDBusInterface::openTaskSwitcher()
{
    Q_D(EmbeddedCompositorDBusInterface);
    d->m_taskSwitcherIface.Open();
}

void EmbeddedCompositorDBusInterface::closeTaskSwitcher()
{
    Q_D(EmbeddedCompositorDBusInterface);
    d->m_taskSwitcherIface.Close();
}

void EmbeddedCompositorDBusInterface::showGlobalOverlay(const QString &message)
{
    Q_D(EmbeddedCompositorDBusInterface);
    d->m_overlayIface.Show(message);
}

void EmbeddedCompositorDBusInterface::hideGlobalOverlay()
{
    Q_D(EmbeddedCompositorDBusInterface);
    d->m_overlayIface.Hide();
}

uint EmbeddedCompositorDBusInterface::notify(const QString &summary, const QString &body, const QStringList &actions)
{
    Q_D(EmbeddedCompositorDBusInterface);
    QStringList actionList;
    actionList.reserve(actions.count() * 2);
    // Notification actions are pairs of name - label.
    // For now, use the label as action name.
    for (const QString &action : actions) {
        actionList << action << action;
    }

    const auto reply = d->m_notificationsIface.Notify(
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

void EmbeddedCompositorDBusInterface::setOrientation(const QString &orientation)
{
    Q_D(EmbeddedCompositorDBusInterface);
    d->m_screenIface.setOrientation(orientation);
}
