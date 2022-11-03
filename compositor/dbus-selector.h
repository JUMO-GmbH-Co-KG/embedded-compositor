#ifndef DBUSSELECTOR_H
#define DBUSSELECTOR_H
#include <QDBusConnection>

inline QDBusConnection getBus() {
#ifdef USE_SYSTEM_BUS
  return QDBusConnection::systemBus();
#else
  return QDBusConnection::sessionBus();
#endif
}

constexpr QDBusConnection::BusType defaultBus =
#ifdef USE_SYSTEM_BUS
    QDBusConnection::BusType::SystemBus;
#else
    QDBusConnection::BusType::SessionBus;
#endif
#endif // DBUSSELECTOR_H
