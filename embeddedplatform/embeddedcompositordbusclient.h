// SPDX-License-Identifier: LGPL-3.0-only

#ifndef DBUSCLIENT_H
#define DBUSCLIENT_H

#include <QObject>
#include <QDBusConnection>
#include <QScopedPointer>

inline QDBusConnection getBus() {
#ifdef USE_SYSTEM_BUS
  return QDBusConnection::systemBus();
#else
  return QDBusConnection::sessionBus();
#endif
}
class EmbeddedCompositorDBusInterfacePrivate;

class EmbeddedCompositorDBusInterface : public QObject
{
    Q_OBJECT

    Q_DECLARE_PRIVATE_D(m_d, EmbeddedCompositorDBusInterface)
    QScopedPointer<EmbeddedCompositorDBusInterfacePrivate> m_d;
public:
    explicit EmbeddedCompositorDBusInterface(QObject *parent = nullptr);
    ~EmbeddedCompositorDBusInterface() override;

    Q_INVOKABLE void openTaskSwitcher();
    Q_INVOKABLE void closeTaskSwitcher();

    Q_INVOKABLE void showGlobalOverlay(const QString &message);
    Q_INVOKABLE void hideGlobalOverlay();
    Q_INVOKABLE uint notify(const QString &summary, const QString &body, const QStringList &actions);

    Q_INVOKABLE void setOrientation(const QString &orientation);

Q_SIGNALS:
    void notificationActionInvoked(unsigned int id, const QString &action);
    void notificationClosed(unsigned int id, unsigned int);

};

#endif // DBUSCLIENT_H
