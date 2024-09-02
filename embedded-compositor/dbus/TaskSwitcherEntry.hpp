// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QObject>
#include <QDBusArgument>
#include <QDBusContext>


struct TaskSwitcherEntry
{
    QString uuid;
    QString appId;
    QString appLabel;
    QString appIcon;
    QString label;
    QString icon;
    uint32_t pid;
    uint32_t sortIndex;
    QVariantMap args; // for future extension
};

Q_DECLARE_METATYPE(TaskSwitcherEntry)
Q_DECLARE_METATYPE(QList<TaskSwitcherEntry>)

QDBusArgument &operator<<(QDBusArgument &argument,
                          const TaskSwitcherEntry &entry);

const QDBusArgument &operator>>(const QDBusArgument &argument,
                                TaskSwitcherEntry &entry);
