// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QObject>
#include <QDBusArgument>
#include <QDBusContext>


struct TaskSwitcherEntry
{
    QString uuid;
    bool isPresistent{false};
    QString parentUuid;
    QString label;
    QString icon;
    uint32_t sortIndex{0};
    QVariantMap customData;
};

Q_DECLARE_METATYPE(TaskSwitcherEntry)
Q_DECLARE_METATYPE(QList<TaskSwitcherEntry>)

QDBusArgument &operator<<(QDBusArgument &argument, const TaskSwitcherEntry &entry);
const QDBusArgument &operator>>(const QDBusArgument &argument, TaskSwitcherEntry &entry);
