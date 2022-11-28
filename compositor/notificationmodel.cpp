// SPDX-License-Identifier: GPL-3.0-only

#include "notificationmodel.h"
#include "dbus-selector.h"
#include <QDBusConnection>
#include <QDBusError>
#include <QDBusMessage>
#include <QDebug>

NotificationModel::NotificationModel(QObject *parent)
    : QAbstractListModel(parent),
      dbusInterface(new NotificationDbusInterface(this)),
      connection(QDBusConnection::connectToBus(defaultBus,
                                               NotificationDbusInterface::Name))

{
  connect(this, &QAbstractListModel::rowsInserted, this,
          &NotificationModel::countChanged);
  connect(this, &QAbstractListModel::rowsRemoved, this,
          &NotificationModel::countChanged);
}

NotificationModel::~NotificationModel() {
  qDeleteAll(m_data);
  m_data.clear();
}

void NotificationModel::Add(NotificationData *data) {
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  m_data.append(data);
  endInsertRows();
  connect(data, &NotificationData::dismissed, this,
          &NotificationModel::dismissed);
}

int NotificationModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return m_data.count();
}

QVariant NotificationModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();

  if (role == Qt::DisplayRole) {
    auto data = m_data[index.row()];
    return QVariant::fromValue(data);
  }
  return QVariant();
}

void NotificationModel::classBegin() {}

void NotificationModel::componentComplete() {

  if (!connection.isConnected()) {
    return;
  }
  connection.registerService(NotificationDbusInterface::Name);

  connection.registerObject(NotificationDbusInterface::Path,
                            dbusInterface.data(),
                            QDBusConnection::ExportAllContents);
}

void NotificationModel::dismissed(QString action) {
  NotificationData *data = qobject_cast<NotificationData *>(sender());
  auto idx = m_data.indexOf(data);
  beginRemoveRows(QModelIndex(), idx, idx);
  m_data.removeAt(idx);
  endRemoveRows();

  emit dbusInterface->ActionInvoked(data->id(), action);
  emit dbusInterface->NotificationClosed(data->id(),
                                         NotificationDbusInterface::Dismissed);
}

unsigned int
NotificationDbusInterface::Notify(QString app_name, unsigned int replaces_id,
                                  QString app_icon, QString summary,
                                  QString body, QStringList actions,
                                  QVariantMap hints, int expire_timeout) {
  qDebug() << "got notification " << app_name << replaces_id << app_icon
           << summary << body << actions << hints << expire_timeout
           << " ID: " << m_idCounter;
  auto data = new NotificationData{app_name, replaces_id,    app_icon,
                                   summary,  body,           actions,
                                   hints,    expire_timeout, m_idCounter};
  m_model->Add(data);
  m_idCounter++;
  return data->id();
}

QStringList NotificationDbusInterface::GetCapabilities() {
  return QStringList{
      "actions", // The server will provide the specified actions to the user.
      // Even if this cap is missing, actions may still be
      // specified by the client, however the server is free to
      // ignore them.
      "body", // Supports body text. Some implementations may only show the
              // summary (for instance, onscreen displays, marquee/scrollers)
  };
  /*
      "action-icons", // Supports using icons instead of text for displaying
                      // actions. Using icons for actions must be enabled on a
                      // per-notification basis using the "action-icons" hint.
      "body-hyperlinks", //	The server supports hyperlinks in the
                         //notifications.
      "body-images",     //	The server supports images in the notifications.
      "body-markup",     //	Supports markup in the body text. If marked up
                     //text is sent to a server that does not give this cap,
                     //the markup will show through as regular text so must be
                     //stripped clientside.
      "icon-multi",  //	The server will render an animation of all the
                     //frames in a given image array. The client may still
                     //specify multiple frames even if this cap and/or
                     //"icon-static" is missing, however the server is free to
                     //ignore them and use only the primary frame.
      "icon-static", //	Supports display of exactly 1 frame of any given
                     //image array. This value is mutually exclusive with
                     //"icon-multi", it is a protocol error for the server to
                     //specify both.
      "persistence", //	The server supports persistence of
                     //notifications. Notifications will be retained until
                     //they are acknowledged or removed by the user or
                     //recalled by the sender. The presence of this capability
                     //allows clients to depend on the server to ensure a
                     //notification is seen and eliminate the need for the
                     //client to display a reminding function (such as a
                     //status icon) of its own.
      "sound", // The server supports sounds on notifications. If returned,
               // the server must support the "sound-file" and
               // "suppress-sound" hints.
  */
}

void NotificationData::dismiss(QString action) { emit dismissed(action); }

unsigned int NotificationModel::count() const { return m_data.count(); }
