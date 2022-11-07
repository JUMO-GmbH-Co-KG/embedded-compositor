#ifndef DBUSINTERFACE_H
#define DBUSINTERFACE_H
#include <QDebug>
#include <QObject>
#include <QQmlParserStatus>
#include <QTimer>

#include <QDBusConnection>
#include <QDBusError>

bool InitDbusConnection(QString serviceName);

class TaskSwitcherInterface : public QObject, public QQmlParserStatus {
  Q_OBJECT
  bool m_valid = false;

public:
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
  bool m_valid = false;

public:
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
#endif // DBUSINTERFACE_H
