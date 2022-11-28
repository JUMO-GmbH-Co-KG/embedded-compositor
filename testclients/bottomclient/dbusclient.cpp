// SPDX-License-Identifier: LGPL-3.0-only

#include "dbusclient.h"

DBusClient::DBusClient(QObject *parent) : QObject{parent} {
  QDBusConnection::sessionBus().connect(
      "org.freedesktop.Notifications", "/org/freedesktop/Notifications",
      "org.freedesktop.Notifications", "ActionInvoked", this,
      SIGNAL(notificationActionInvoked(unsigned int, QString)));
  QDBusConnection::sessionBus().connect(
      "org.freedesktop.Notifications", "/org/freedesktop/Notifications",
      "org.freedesktop.Notifications", "NotificationClosed", this,
      SIGNAL(notificationClosed(unsigned int, unsigned int)));
  QDBusConnection::sessionBus().connect(
      "org.freedesktop.Notifications", "/org/freedesktop/Notifications",
      "org.freedesktop.Notifications", "ActionInvoked", this,
      SLOT(notificationActionInvoked(unsigned int, QString)));
}

uint DBusClient::notify(QString summary, QString body, QStringList actions) {
    auto message = notifications.callWithArgumentList(
        QDBus::CallMode::Block, "Notify",
        {"bottom client", 0u, "no-icon", summary, body, actions, QVariantMap(),
            -1

        });

    return message.arguments().first().toUInt();
}
