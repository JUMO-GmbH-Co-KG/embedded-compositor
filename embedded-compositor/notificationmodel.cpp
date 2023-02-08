// SPDX-License-Identifier: GPL-3.0-only

#include "notificationmodel.h"

#include "dbus-selector.h"

#include <QDBusConnection>

#include <algorithm>

#include "notifications_adaptor.h"

Q_LOGGING_CATEGORY(compositorNotification, "compositor.notification")

NotificationModel::NotificationModel(QObject *parent)
    : QAbstractListModel(parent)
{
    new NotificationsAdaptor(this);

    connect(this, &NotificationModel::rowsInserted, this, &NotificationModel::countChanged);
    connect(this, &NotificationModel::rowsRemoved, this, &NotificationModel::countChanged);
}

void NotificationModel::classBegin()
{
}

void NotificationModel::componentComplete()
{
    auto connection = getBus();

    if (!connection.registerObject(QStringLiteral("/org/freedesktop/Notifications"), this)) {
        qCWarning(compositorNotification) << "Failed to register notifications object";
        return;
    }
    if (!connection.registerService(QStringLiteral("org.freedesktop.Notifications"))) {
        qCWarning(compositorNotification) << "Failed to register notifications service";
        return;
    }

    m_valid = true;
    Q_EMIT validChanged();
}

bool NotificationModel::valid() const
{
    return m_valid;
}

int NotificationModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_notifications.count();
}

QVariant NotificationModel::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index, QAbstractItemModel::CheckIndexOption::IndexIsValid)) {
        return QVariant();
    }

    const auto &notification = m_notifications.at(index.row());

    switch (role) {
    case IdRole:
        return notification.id;
    case SummaryRole:
        return notification.summary;
    case BodyRole:
        return notification.body;
    case ActionNamesRole:
        return notification.actionNames;
    case ActionLabelsRole:
        return notification.actionLabels;
    }

    return QVariant();
}

QHash<int, QByteArray> NotificationModel::roleNames() const
{
    return {
        {IdRole, QByteArrayLiteral("notificationId")}, // id is a reserved QML keyword.
        {SummaryRole, QByteArrayLiteral("summary")},
        {BodyRole, QByteArrayLiteral("body")},
        {ActionNamesRole, QByteArrayLiteral("actionNames")},
        {ActionLabelsRole, QByteArrayLiteral("actionLabels")},
    };
}

void NotificationModel::dismiss(const QModelIndex &index)
{
    if (!checkIndex(index, QAbstractItemModel::CheckIndexOption::IndexIsValid)) {
        return;
    }

    const auto id = m_notifications.at(index.row()).id;

    beginRemoveRows(QModelIndex(), index.row(), index.row());
    m_notifications.removeAt(index.row());
    endRemoveRows();

    Q_EMIT NotificationClosed(id, static_cast<uint>(CloseReason::Dismissed));
}

void NotificationModel::invokeAction(const QModelIndex &index, const QString &action)
{
    if (!checkIndex(index, QAbstractItemModel::CheckIndexOption::IndexIsValid)) {
        return;
    }

    const auto &notification = m_notifications.at(index.row());
    Q_ASSERT(notification.actionNames.contains(action));

    Q_EMIT ActionInvoked(notification.id, action);
    dismiss(index);
}

uint NotificationModel::Notify(const QString &app_name, uint replaces_id, const QString &app_icon, const QString &summary, const QString &body, const QStringList &actions, const QVariantMap &hints, int timeout)
{
    qCDebug(compositorNotification) << "Got notification with summary" << app_name << replaces_id << app_icon << summary << body << actions << hints << timeout;

    Notification notification;
    notification.id = ++m_idCounter;
    if (!notification.id) { // Never return zero from this call.
        ++notification.id;
    }

    notification.summary = summary;
    notification.body = body;

    if (actions.count() % 2 != 0) {
        qCWarning(compositorNotification) << "Received an odd number of actions, this is a client bug:" << actions;
    } else {
        notification.actionNames.reserve(actions.count() / 2);
        notification.actionLabels.reserve(actions.count() / 2);
        // Actions are pairs of name - label.
        for (int i = 0; i < actions.count(); i += 2) {
            const auto &name = actions.at(i);
            const auto &label = actions.at(i + 1);
            notification.actionNames.append(name);
            notification.actionLabels.append(label);
        }
    }

    if (replaces_id) {
        auto it = std::find_if(m_notifications.cbegin(), m_notifications.cend(), [replaces_id](const Notification &notification) {
            return notification.id == replaces_id;
        });

        if (it != m_notifications.cend()) {
            const auto row = std::distance(m_notifications.cbegin(), it);

            notification.id = replaces_id;

            m_notifications[row] = notification;
            const QModelIndex idx = index(row, 0);
            Q_EMIT dataChanged(idx, idx);

            return notification.id;
        } else {
            qCWarning(compositorNotification) << "Don't know notification id" << replaces_id << "to replace, issuing new notification, this is a client bug.";
        }
    }

    beginInsertRows(QModelIndex(), m_notifications.count(), m_notifications.count());
    m_notifications.append(notification);
    endInsertRows();

    return notification.id;
}

void NotificationModel::CloseNotification(uint id)
{
    qCDebug(compositorNotification) << "Close notification requested for" << id;

    auto it = std::find_if(m_notifications.cbegin(), m_notifications.cend(), [id](const Notification &notification) {
        return notification.id == id;
    });

    if (it == m_notifications.cend()) {
        return;
    }

    const auto row = std::distance(m_notifications.cbegin(), it);

    beginRemoveRows(QModelIndex(), row, row);
    m_notifications.removeAt(row);
    endRemoveRows();

    Q_EMIT NotificationClosed(it->id, static_cast<uint>(CloseReason::Revoked));
}

QStringList NotificationModel::GetCapabilities() const
{
    return QStringList{
        QStringLiteral("actions"), // The server will provide the specified actions to the user.
        // Even if this cap is missing, actions may still be
        // specified by the client, however the server is free to
        // ignore them.
        QStringLiteral("body"), // Supports body text. Some implementations may only show the
                // summary (for instance, onscreen displays, marquee/scrollers)
    };
    /*
        "action-icons", // Supports using icons instead of text for displaying
                        // actions. Using icons for actions must be enabled on a
                        // per-notification basis using the "action-icons" hint.
        "body-hyperlinks", //	The server supports hyperlinks in the
                           //notifications.
        "body-images",     //	The server supports images in the notifications.
        "body-markup",     //	Supports markup in the body text. If marked up
                       //text is sent to a server that does not give this cap,
                       //the markup will show through as regular text so must be
                       //stripped clientside.
        "icon-multi",  //	The server will render an animation of all the
                       //frames in a given image array. The client may still
                       //specify multiple frames even if this cap and/or
                       //"icon-static" is missing, however the server is free to
                       //ignore them and use only the primary frame.
        "icon-static", //	Supports display of exactly 1 frame of any given
                       //image array. This value is mutually exclusive with
                       //"icon-multi", it is a protocol error for the server to
                       //specify both.
        "persistence", //	The server supports persistence of
                       //notifications. Notifications will be retained until
                       //they are acknowledged or removed by the user or
                       //recalled by the sender. The presence of this capability
                       //allows clients to depend on the server to ensure a
                       //notification is seen and eliminate the need for the
                       //client to display a reminding function (such as a
                       //status icon) of its own.
        "sound", // The server supports sounds on notifications. If returned,
                 // the server must support the "sound-file" and
                 // "suppress-sound" hints.
    */
}

QString NotificationModel::GetServerInformation(QString &vendor, QString &version, QString &spec_version) const
{
    vendor = QStringLiteral("EmbeddedCompositor");
    version = QStringLiteral("0.0.0.1");
    spec_version = QStringLiteral("1.2");
    return QStringLiteral("EmbeddedCompositor"); // name
}
