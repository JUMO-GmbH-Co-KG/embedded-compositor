// SPDX-License-Identifier: GPL-3.0-only

#include "DBusInterface.hpp"
#include "dbus-selector.h"
#include "embeddedshellextension.h"

#include <QAbstractListModel>
#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>
#include <QMetaObject>
#include <QtDBus/QtDBus>

Q_LOGGING_CATEGORY(compositorDBus, "compositor.dbus")

DBusInterface::DBusInterface(const QString &objectPath, QObject *parent)
    : QObject(parent), m_objectPath(objectPath)
{
}

void DBusInterface::classBegin()
{
}

void DBusInterface::componentComplete()
{
  Q_ASSERT_X(findChild<QDBusAbstractAdaptor *>(), "DBusInterface",
             "DBusInterface constructed without attached DBus adaptor");

  const bool valid = busConnection().registerObject(m_objectPath, this);
  if (!valid) {
    qCWarning(compositorDBus) << "Failed to register interface"
                              << interfaceName() << "on" << objectPath();
    return;
  }

  const QMetaMethod propertyChangedSlot =
      metaObject()->method(metaObject()->indexOfMethod("onPropertyChanged()"));
  Q_ASSERT(propertyChangedSlot.isValid());

  // Qt DBus does not automatically emit the properties changed signal, hook it
  // up manually.
  const auto mo = metaObject();
  for (int i = mo->propertyOffset(); i < mo->propertyCount(); ++i)
  {
    const auto prop = mo->property(i);
    if (!prop.hasNotifySignal()) {
      continue;
    }

    const auto notifySignal = prop.notifySignal();
    const bool ok = connect(this, notifySignal, this, propertyChangedSlot);
    Q_ASSERT(ok);
  }

  qCDebug(compositorDBus) << "Registered interface" << interfaceName() << "on"
                          << objectPath();
  m_valid = valid;
  emit validChanged(valid);
}

QDBusConnection DBusInterface::busConnection() const
{
    return getBus();
}

QString DBusInterface::interfaceName() const
{
  const auto *adaptor = findChild<QDBusAbstractAdaptor *>();
  const auto mo = adaptor->metaObject();
  const int classInfoIdx = mo->indexOfClassInfo("D-Bus Interface");
  Q_ASSERT(classInfoIdx > -1);
  const auto classInfo = mo->classInfo(classInfoIdx);
  return QString::fromUtf8(classInfo.value());
}

QString DBusInterface::objectPath() const
{
    return m_objectPath;
}

bool DBusInterface::valid() const
{
    return m_valid;
}

void DBusInterface::onPropertyChanged()
{
  const auto mo = metaObject();

  Q_ASSERT(sender() == this);
  const auto emittedSignal = mo->method(senderSignalIndex());

  QMetaProperty changedProperty;

  // Find the property that belongs to this signal and emit the DBus signal.
  for (int i = mo->propertyOffset(); i < mo->propertyCount(); ++i)
  {
    const auto prop = mo->property(i);
    if (!prop.hasNotifySignal())
    {
      continue;
    }

    if (prop.notifySignal() == emittedSignal)
    {
      changedProperty = prop;
      break;
    }
  }

  Q_ASSERT(changedProperty.isValid());

  QDBusMessage signal = QDBusMessage::createSignal(
      objectPath(), QStringLiteral("org.freedesktop.DBus.Properties"),
      QStringLiteral("PropertiesChanged"));
  signal.setArguments({
      interfaceName(),
      QVariantMap{{QString::fromUtf8(changedProperty.name()),
                   changedProperty.read(this)}},
      QStringList(), // invalidated.
  });

  busConnection().send(signal);
}

bool InitDbusConnection(QString serviceName)
{
  auto bus = getBus();
  if (!bus.isConnected())
  {
    qWarning() << "Cannot connect to the D-Bus.\n"
                  "To start it, run:\n"
                  "\teval `dbus-launch --auto-syntax`\n";
    return false;
  }
  if (!bus.registerService(serviceName))
  {
    qDebug() << bus.lastError().message();
    return false;
  }
  return true;
}
