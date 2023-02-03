// SPDX-License-Identifier: GPL-3.0-only

#ifndef DBUSINTERFACE_H
#define DBUSINTERFACE_H

#include <QDBusConnection>
#include <QDBusError>
#include <QObject>
#include <QQmlParserStatus>
#include <QTimer>

bool InitDbusConnection(QString serviceName);

class TaskSwitcherInterface : public QObject, public QQmlParserStatus {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "com.embeddedcompositor.taskswitcher")
  bool m_valid = false;

public:
  Q_INTERFACES(QQmlParserStatus)
  TaskSwitcherInterface() {}
  virtual ~TaskSwitcherInterface() override {}

  Q_PROPERTY(bool valid READ valid NOTIFY validChanged)
  bool valid() const { return m_valid; }

  // QQmlParserStatus interface
  void classBegin() override {}
  void componentComplete() override;

public slots:
  Q_SCRIPTABLE void Open() { emit openRequested(); }
  Q_SCRIPTABLE void Close() { emit closeRequested(); }

signals:
  void validChanged();
  void openRequested();
  void closeRequested();
};

class GlobalOverlayInterface : public QObject, public QQmlParserStatus {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "com.embeddedcompositor.globaloverlay")
  bool m_valid = false;

public:
  Q_INTERFACES(QQmlParserStatus)
  GlobalOverlayInterface() {}
  virtual ~GlobalOverlayInterface() override {}

  Q_PROPERTY(bool valid READ valid NOTIFY validChanged)
  bool valid() const { return m_valid; }

  // QQmlParserStatus interface
  void classBegin() override {}
  void componentComplete() override;

public slots:
  Q_SCRIPTABLE void Show(QString message) { emit showRequested(message); }
  Q_SCRIPTABLE void Hide() { emit hideRequested(); }

signals:
  void validChanged();
  void showRequested(QString message);
  void hideRequested();
};

class CompositorScreenInterface : public QObject, public QQmlParserStatus {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "com.embeddedcompositor.screen")
  bool m_valid = false;

public:
  Q_INTERFACES(QQmlParserStatus)
  CompositorScreenInterface();
  virtual ~CompositorScreenInterface() override {}

  Q_PROPERTY(bool valid READ valid NOTIFY validChanged)
  bool valid() const { return m_valid; }

  // QQmlParserStatus interface
  void classBegin() override {}
  void componentComplete() override;

  Q_PROPERTY(QString orientation READ orientation WRITE setOrientation NOTIFY
                 orientationChanged)

  const QString &orientation() const;
  void setOrientation(const QString &newOrientation);

signals:
  void validChanged();
  void orientationChanged(const QString &orientation);

private:
  QString m_orientation = "0";
};
#endif // DBUSINTERFACE_H
