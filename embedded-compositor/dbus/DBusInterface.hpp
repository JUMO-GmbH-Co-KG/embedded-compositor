// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QDBusConnection>
#include <QDBusContext>
#include <QLoggingCategory>
#include <QObject>
#include <QQmlParserStatus>

Q_DECLARE_LOGGING_CATEGORY(compositorDBus)

bool InitDbusConnection(QString serviceName);


class DBusInterface : public QObject,
                      public QQmlParserStatus,
                      protected QDBusContext
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

signals:
  void validChanged(bool valid);

protected:
  DBusInterface(const QString &objectPath, QObject *parent = nullptr);

  // QQmlParserStatus
  void classBegin() override;
  void componentComplete() override;

private slots:
  void onPropertyChanged();

private:
  QString m_objectPath;
  bool m_valid = false;
};
