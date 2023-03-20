// SPDX-License-Identifier: GPL-3.0-only

#ifndef NOTIFICATIONMODEL_H
#define NOTIFICATIONMODEL_H

#include <QAbstractListModel>
#include <QLoggingCategory>
#include <QQmlParserStatus>
#include <QVector>

Q_DECLARE_LOGGING_CATEGORY(compositorNotification);

struct Notification
{
    uint id = 0;
    QString summary;
    QString body;
    QStringList actionNames;
    QStringList actionLabels;
};
Q_DECLARE_TYPEINFO(Notification, Q_MOVABLE_TYPE);

class NotificationModel : public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)

public:
    NotificationModel(QObject *parent = nullptr);
    ~NotificationModel() override = default;

    enum Roles {
        SummaryRole = Qt::DisplayRole,
        IdRole = Qt::UserRole,
        BodyRole,
        ActionNamesRole,
        ActionLabelsRole
    };
    Q_ENUM(Roles)

    enum class CloseReason {
        Expired = 1,
        Dismissed = 2,
        Revoked = 3
    };

    bool valid() const;
    Q_SIGNAL void validChanged();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void classBegin() override;
    void componentComplete() override;

    Q_INVOKABLE void dismiss(const QModelIndex &index);
    Q_INVOKABLE void invokeAction(const QModelIndex &index, const QString &action);

    // DBus
    uint Notify(const QString &app_name, uint replaces_id, const QString &app_icon, const QString &summary, const QString &body, const QStringList &actions, const QVariantMap &hints, int timeout);
    void CloseNotification(uint id);
    QStringList GetCapabilities() const;
    QString GetServerInformation(QString &vendor, QString &version, QString &spec_version) const;

Q_SIGNALS:
    void countChanged();

    // DBus
    void ActionInvoked(uint id, const QString &action_key);
    void ActivationToken(uint id, const QString &activation_token);
    void NotificationClosed(uint id, uint reason);

private:
    QVector<Notification> m_notifications;
    uint m_idCounter = 0;
    bool m_valid = false;
};

#endif // NOTIFICATIONMODEL_H
