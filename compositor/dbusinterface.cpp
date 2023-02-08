// SPDX-License-Identifier: GPL-3.0-only

#include "dbusinterface.h"
#include "dbus-selector.h"
#include <QDBusConnection>
#include <QDebug>
#include <QMetaObject>

#include "globaloverlay_adaptor.h"
#include "screen_adaptor.h"
#include "taskswitcher_adaptor.h"

Q_LOGGING_CATEGORY(compositorDBus, "compositor.dbus")

DBusInterface::DBusInterface(const QString &objectPath, QObject *parent)
    : QObject(parent)
    , m_objectPath(objectPath)
{
}

void DBusInterface::classBegin()
{
}

void DBusInterface::componentComplete()
{
    Q_ASSERT_X(findChild<QDBusAbstractAdaptor *>(), "DBusInterface", "DBusInterface constructed without attached DBus adaptor");

    const bool valid = busConnection().registerObject(m_objectPath, this);
    if (!valid) {
        qCWarning(compositorDBus) << "Failed to register interface" << interfaceName() << "on" << objectPath();
        return;
    }

    const QMetaMethod propertyChangedSlot = metaObject()->method(metaObject()->indexOfMethod("onPropertyChanged()"));
    Q_ASSERT(propertyChangedSlot.isValid());

    // Qt DBus does not automatically emit the properties changed signal, hook it up manually.
    const auto mo = metaObject();
    for (int i = mo->propertyOffset(); i < mo->propertyCount(); ++i) {
        const auto prop = mo->property(i);
        if (!prop.hasNotifySignal()) {
            continue;
        }

        const auto notifySignal = prop.notifySignal();
        const bool ok = connect(this, notifySignal, this, propertyChangedSlot);
        Q_ASSERT(ok);
    }

    qCDebug(compositorDBus) << "Registered interface" << interfaceName() << "on" << objectPath();
    m_valid = valid;
    Q_EMIT validChanged(valid);
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
    for (int i = mo->propertyOffset(); i < mo->propertyCount(); ++i) {
        const auto prop = mo->property(i);
        if (!prop.hasNotifySignal()) {
            continue;
        }

        if (prop.notifySignal() == emittedSignal) {
            changedProperty = prop;
            break;
        }
    }

    Q_ASSERT(changedProperty.isValid());

    QDBusMessage signal = QDBusMessage::createSignal(objectPath(),
                                                     QStringLiteral("org.freedesktop.DBus.Properties"),
                                                     QStringLiteral("PropertiesChanged"));
    signal.setArguments({
        interfaceName(),
        QVariantMap{
            { QString::fromUtf8(changedProperty.name()), changedProperty.read(this) }
        },
        QStringList(), // invalidated.
    });

    busConnection().send(signal);
}

bool InitDbusConnection(QString serviceName) {
  auto bus = getBus();
  if (!bus.isConnected()) {
    qWarning() << "Cannot connect to the D-Bus.\n"
                  "To start it, run:\n"
                  "\teval `dbus-launch --auto-syntax`\n";
    return false;
  }
  if (!bus.registerService(serviceName)) {
    qDebug() << bus.lastError().message();
    return false;
  }
  return true;
}

TaskSwitcherInterface::TaskSwitcherInterface(QObject *parent)
    : DBusInterface(QStringLiteral("/taskswitcher"), parent)
{
    new TaskswitcherAdaptor(this);
}

void TaskSwitcherInterface::Open()
{
    Q_EMIT openRequested();
}

void TaskSwitcherInterface::Close()
{
    Q_EMIT closeRequested();
}

GlobalOverlayInterface::GlobalOverlayInterface(QObject *parent)
    : DBusInterface(QStringLiteral("/globaloverlay"), parent)
{
    new GlobaloverlayAdaptor(this);
}

void GlobalOverlayInterface::Show(const QString &message)
{
    Q_EMIT showRequested(message);
}

void GlobalOverlayInterface::Hide()
{
    Q_EMIT hideRequested();
}

CompositorScreenInterface::CompositorScreenInterface(QObject *parent)
    : DBusInterface(QStringLiteral("/screen"), parent)
{
    new ScreenAdaptor(this);

    m_orientation = qEnvironmentVariable("SCREEN_ORIENTATION");
    if (m_orientation.isEmpty()) {
        m_orientation = QStringLiteral("0");
    }
}

QString CompositorScreenInterface::orientation() const
{
    return m_orientation;
}

void CompositorScreenInterface::setOrientation(const QString &orientation)
{
    if (m_orientation != orientation) {
        m_orientation = orientation;
        Q_EMIT orientationChanged(orientation);
    }
}
