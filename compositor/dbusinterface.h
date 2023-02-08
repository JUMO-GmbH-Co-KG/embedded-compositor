// SPDX-License-Identifier: GPL-3.0-only

#ifndef DBUSINTERFACE_H
#define DBUSINTERFACE_H

#include <QDBusConnection>
#include <QDBusContext>
#include <QLoggingCategory>
#include <QObject>
#include <QQmlParserStatus>

Q_DECLARE_LOGGING_CATEGORY(compositorDBus)

bool InitDbusConnection(QString serviceName);

class DBusInterface : public QObject, public QQmlParserStatus, protected QDBusContext
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QString interfaceName READ interfaceName CONSTANT)
    Q_PROPERTY(QString objectPath READ objectPath CONSTANT)
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)

public:
    ~DBusInterface() override = default;

    QDBusConnection busConnection() const;
    QString interfaceName() const;
    QString objectPath() const;

    bool valid() const;
    Q_SIGNAL void validChanged(bool valid);

protected:
    DBusInterface(const QString &objectPath, QObject *parent = nullptr);

    // QQmlParserStatus
    void classBegin() override;
    void componentComplete() override;

private Q_SLOTS:
    void onPropertyChanged();

private:
    QString m_objectPath;
    bool m_valid = false;
};

class TaskSwitcherInterface : public DBusInterface
{
    Q_OBJECT

public:
    TaskSwitcherInterface(QObject *parent = nullptr);
    ~TaskSwitcherInterface() override = default;

    // DBus
    void Open();
    void Close();

Q_SIGNALS:
    void openRequested();
    void closeRequested();
};

class GlobalOverlayInterface : public DBusInterface
{
    Q_OBJECT

public:
    GlobalOverlayInterface(QObject *parent = nullptr);
    ~GlobalOverlayInterface() override = default;

    // DBus
    void Show(const QString &message);
    void Hide();

Q_SIGNALS:
    void showRequested(const QString &message);
    void hideRequested();
};

class CompositorScreenInterface : public DBusInterface
{
    Q_OBJECT

    Q_PROPERTY(QString orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)

public:
    CompositorScreenInterface(QObject *parent = nullptr);
    ~CompositorScreenInterface() override = default;

    QString orientation() const;
    void setOrientation(const QString &orientation);

Q_SIGNALS:
    void orientationChanged(const QString &orientation);

private:
    QString m_orientation;
};

#endif // DBUSINTERFACE_H
