#ifndef NOTIFICATIONMODEL_H
#define NOTIFICATIONMODEL_H

#include <QAbstractItemModel>

#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>
#include <QQmlParserStatus>

class NotificationModel;

class NotificationDbusInterface : public QObject {

  Q_OBJECT
#define IFACE_STR "org.freedesktop.Notifications"
  Q_CLASSINFO("D-Bus Interface", IFACE_STR)

  NotificationModel *m_model;
  unsigned int m_idCounter = 1;

public:
  enum CloseReason {
    Expired = 1,
    Dismissed = 2,
    CloseCall = 3,
    UndefinedReserved = 4
  };

  static constexpr auto Interface = IFACE_STR;
  static constexpr auto Name = IFACE_STR;
  static constexpr auto Path = "/org/freedesktop/Notifications";
  NotificationDbusInterface(NotificationModel *model) : m_model(model) {}

public slots:

  unsigned int Notify(QString app_name, unsigned int replaces_id,
                      QString app_icon, QString summary, QString body,
                      QStringList actions, QVariantMap hints,
                      int expire_timeout);
  QStringList GetCapabilities();

  void GetServerInformation(QString &name, QString &vendor, QString &version,
                            QString &spec_version) {
    qDebug() << "get info...";
    name = "foo";
    vendor = "bar";
    version = "1.2.3.4";
    spec_version = "1.2";
  }
signals:
  void NotificationClosed(unsigned int id, unsigned int reason);
  void ActionInvoked(unsigned int id, QString action_key);
  void ActivationToken(unsigned int id, QString activation_token);
};

class NotificationData : public QObject {
  Q_OBJECT
  QString m_appName;
  QString m_appIcon;
  QString m_summary;
  QString m_body;
  QStringList m_actions;
  QVariantMap m_hints;
  unsigned int m_replacesId;
  int m_expireTimeout;
  unsigned int m_id;
  Q_PROPERTY(QString appName READ appName NOTIFY dataChanged)
  Q_PROPERTY(unsigned int replacesId READ replacesId NOTIFY dataChanged)
  Q_PROPERTY(QString appIcon READ appIcon NOTIFY dataChanged)
  Q_PROPERTY(QString summary READ summary NOTIFY dataChanged)
  Q_PROPERTY(QString body READ body NOTIFY dataChanged)
  Q_PROPERTY(QStringList actions READ actions NOTIFY dataChanged)
  Q_PROPERTY(QVariantMap hints READ hints NOTIFY dataChanged)
  Q_PROPERTY(int expireTimeout READ expireTimeout NOTIFY dataChanged)
  Q_PROPERTY(unsigned int id READ id NOTIFY dataChanged)

public:
  NotificationData(QString appName, unsigned int replacesId, QString appIcon,
                   QString summary, QString body, QStringList actions,
                   QVariantMap hints, int expireTimeout, unsigned int id)
      : m_appName(appName), m_appIcon(appIcon), m_summary(summary),
        m_body(body), m_actions(actions), m_hints(hints),
        m_replacesId(replacesId), m_expireTimeout(expireTimeout), m_id(id) {}

  ~NotificationData() override {}

  const QString &appName() const { return m_appName; }
  unsigned int replacesId() const { return m_replacesId; }
  const QString &appIcon() const { return m_appIcon; }
  const QString &summary() const { return m_summary; }
  const QString &body() const { return m_body; }
  const QStringList &actions() const { return m_actions; }
  const QVariantMap &hints() const { return m_hints; }
  int expireTimeout() const { return m_expireTimeout; }
  unsigned int id() const { return m_id; }

public slots:
  void dismiss(QString action);
signals:
  void dataChanged();
  void timeout();
  void dismissed(QString action);
};

class NotificationModel : public QAbstractListModel, public QQmlParserStatus {
  Q_OBJECT
  QScopedPointer<NotificationDbusInterface> dbusInterface;
  QDBusConnection connection;
  QList<NotificationData *> m_data;
  Q_PROPERTY(unsigned int count READ count NOTIFY countChanged)

public:
  explicit NotificationModel(QObject *parent = nullptr);
  virtual ~NotificationModel() override;

  void Add(NotificationData *data);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  QHash<int, QByteArray> roleNames() const override {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "data";
    return roles;
  }

  void classBegin() override;
  void componentComplete() override;
  unsigned int count() const;

signals:
  void countChanged();

private slots:
  void dismissed(QString action);
};

#endif // NOTIFICATIONMODEL_H
