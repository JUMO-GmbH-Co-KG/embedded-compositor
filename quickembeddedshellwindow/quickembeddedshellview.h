// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "quickembeddedshellsurface.h"
#include "quickembeddedshellwindow_global.h"

#include <QQuickItem>

class EMBEDDEDSHELLWINDOW_EXPORT QuickEmbeddedShellView : public QQuickItem
{
  Q_OBJECT
  QML_NAMED_ELEMENT(View)

  Q_PROPERTY(QuickEmbeddedShellSurface *surface READ surface WRITE setSurface NOTIFY surfaceChanged)
  Q_PROPERTY(bool selected READ selected NOTIFY selectedChanged)

  Q_PROPERTY(QString appId READ appId WRITE setAppId NOTIFY appIdChanged)
  Q_PROPERTY(QString appLabel READ appLabel WRITE setAppLabel NOTIFY appLabelChanged)
  Q_PROPERTY(QString appIcon READ appIcon WRITE setAppIcon NOTIFY appIconChanged)
  Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
  Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
  Q_PROPERTY(quint32 sortIndex READ sortIndex WRITE setSortIndex NOTIFY sortIndexChanged)
  Q_PROPERTY(QVariant customData READ customData WRITE setCustomData NOTIFY customDataChanged)

public:
  explicit QuickEmbeddedShellView(QQuickItem *parent = nullptr);

  void componentComplete() override;

  QuickEmbeddedShellSurface *surface() const;
  void setSurface(QuickEmbeddedShellSurface *surface);

  bool selected() const;

  QString appId() const;
  void setAppId(const QString &appId);

  QString appLabel() const;
  void setAppLabel(const QString &appLabel);

  QString appIcon() const;
  void setAppIcon(const QString &appIcon);

  QString label() const;
  void setLabel(const QString &label);

  QString icon() const;
  void setIcon(const QString &icon);

  quint32 sortIndex() const;
  void setSortIndex(quint32 sortIndex);

  QVariant customData() const;
  void setCustomData(const QVariant &customData);

signals:
  void surfaceChanged(QuickEmbeddedShellSurface *surface);
  void selectedChanged(bool selected);
  void appIdChanged(const QString &appId);
  void appLabelChanged(const QString &appLabel);
  void appIconChanged(const QString &appIcon);
  void labelChanged(const QString &label);
  void iconChanged(const QString &icon);
  void sortIndexChanged(quint32 sortIndex);
  void customDataChanged(const QVariant &customData);

private:
  void setSelected(bool selected);
  void createView();

  QuickEmbeddedShellSurface *m_surface;
  bool m_selected;

  QString m_appId;
  QString m_appLabel;
  QString m_appIcon;
  QString m_label;
  QString m_icon;
  quint32 m_sortIndex;
  QVariant m_customData;
  bool m_completed;
};
