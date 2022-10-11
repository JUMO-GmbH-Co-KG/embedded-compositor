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
