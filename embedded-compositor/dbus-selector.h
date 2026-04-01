// SPDX-License-Identifier: GPL-3.0-only

#pragma once

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
