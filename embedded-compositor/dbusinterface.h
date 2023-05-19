// SPDX-License-Identifier: GPL-3.0-only

#ifndef DBUSINTERFACE_H
#define DBUSINTERFACE_H

#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusContext>
#include <QLoggingCategory>
#include <QObject>
#include <QQmlParserStatus>

Q_DECLARE_LOGGING_CATEGORY(compositorDBus)

bool InitDbusConnection(QString serviceName);

class DBusInterface : public QObject,
                      public QQmlParserStatus,
                      protected QDBusContext {
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

class QAbstractListModel;

struct TaskSwitcherEntry {
  QString uuid;
  QString label;
  int pid;
};

Q_DECLARE_METATYPE(TaskSwitcherEntry)
Q_DECLARE_METATYPE(QList<TaskSwitcherEntry>)

QDBusArgument &operator<<(QDBusArgument &argument,
                          const TaskSwitcherEntry &entry);

const QDBusArgument &operator>>(const QDBusArgument &argument,
                                TaskSwitcherEntry &entry);

class TaskSwitcherInterface : public DBusInterface {
  Q_OBJECT

public:
  TaskSwitcherInterface(QObject *parent = nullptr);
  ~TaskSwitcherInterface() override = default;

  Q_PROPERTY(QString currentView READ currentView WRITE setCurrentView NOTIFY
                 currentViewChanged)
  Q_PROPERTY(QList<TaskSwitcherEntry> views READ views NOTIFY viewsChanged)
  Q_PROPERTY(QAbstractListModel *viewModel READ viewModel WRITE setViewModel
                 NOTIFY viewModelChanged)

  // DBus
  void Open();
  void Close();

  QString currentView() const;
  void setCurrentView(QString newCurrentView);

  const QList<TaskSwitcherEntry> &views() const;

  QAbstractListModel *viewModel() const;
  void setViewModel(QAbstractListModel *newViewModel);

public slots:

Q_SIGNALS:
  void openRequested();
  void closeRequested();
  void currentViewChanged(QString CurrentView);
  void viewsChanged(const QList<TaskSwitcherEntry> &views);

  void viewModelChanged(QAbstractListModel *viewModel);

private:
  QString m_currentView;
  QList<TaskSwitcherEntry> m_views = {};
  QAbstractListModel *m_viewModel = nullptr;
private slots:
  void publishViews();
};

class GlobalOverlayInterface : public DBusInterface {
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

class CompositorScreenInterface : public DBusInterface {
  Q_OBJECT

  Q_PROPERTY(QString orientation READ orientation WRITE setOrientation NOTIFY
                 orientationChanged)
  Q_PROPERTY(bool screenSaverActive READ screenSaverActive WRITE
                 setScreenSaverActive NOTIFY screenSaverActiveChanged)
  Q_PROPERTY(bool screenSaverEnabled READ screenSaverEnabled WRITE
                 setScreenSaverEnabled NOTIFY screenSaverEnabledChanged)
  Q_PROPERTY(
      int screenSaverTimeoutSeconds READ screenSaverTimeoutSeconds WRITE
          setScreenSaverTimeoutSeconds NOTIFY screenSaverTimeoutSecondsChanged)

public:
  CompositorScreenInterface(QObject *parent = nullptr);
  ~CompositorScreenInterface() override = default;

  QString orientation() const;
  void setOrientation(const QString &orientation);

  bool screenSaverActive() const;
  void setScreenSaverActive(bool newScreenSaverActive);
  void ShowScreenSaver();

  bool screenSaverEnabled() const;
  void setScreenSaverEnabled(bool newScreenSaverEnabled);

  int screenSaverTimeoutSeconds() const;
  void setScreenSaverTimeoutSeconds(int newScreenSaverTimerSeconds);

Q_SIGNALS:
  void orientationChanged(const QString &orientation);
  void screenSaverActiveChanged(bool screenSaverActive);
  void showScreenSaver();
  void screenSaverEnabledChanged(bool screenSaverEnabled);
  void screenSaverTimeoutSecondsChanged(int screenSaverTimerSeconds);

private:
  QString m_orientation;
  bool m_screenSaverActive;
  bool m_screenSaverEnabled;
  int m_screenSaverTimeoutSeconds;
};

#endif // DBUSINTERFACE_H
