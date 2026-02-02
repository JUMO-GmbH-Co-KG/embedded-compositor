// SPDX-License-Identifier: LGPL-3.0-only

#ifndef QUICKEMBEDDEDSHELLVIEW_H
#define QUICKEMBEDDEDSHELLVIEW_H

#include "quickembeddedshellwindow.h"
#include "quickembeddedshellwindow_global.h"

#include <QQuickItem>

class EMBEDDEDSHELLWINDOW_EXPORT QuickEmbeddedShellView : public QQuickItem
{
  Q_OBJECT

  Q_PROPERTY(QuickEmbeddedShellWindow *embeddedShellWindow READ embeddedShellWindow WRITE setEmbeddedShellWindow NOTIFY embeddedShellWindowChanged)
  Q_PROPERTY(bool isCurrentView READ isCurrentView NOTIFY isCurrentViewChanged)

  Q_PROPERTY(QString appId READ appId WRITE setAppId NOTIFY appIdChanged)
  Q_PROPERTY(QString appLabel READ appLabel WRITE setAppLabel NOTIFY appLabelChanged)
  Q_PROPERTY(QString appIcon READ appIcon WRITE setAppIcon NOTIFY appIconChanged)
  Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
  Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
  Q_PROPERTY(quint32 sortIndex READ sortIndex WRITE setSortIndex NOTIFY sortIndexChanged)

public:
  explicit QuickEmbeddedShellView(QQuickItem *parent = nullptr);

  void componentComplete() override;

  QuickEmbeddedShellWindow *embeddedShellWindow() const;
  void setEmbeddedShellWindow(QuickEmbeddedShellWindow *embeddedShellWindow);

  bool isCurrentView() const;

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

signals:
  void embeddedShellWindowChanged(QuickEmbeddedShellWindow *embeddedShellWindow);
  void isCurrentViewChanged(bool isCurrentView);
  void appIdChanged(const QString &appId);
  void appLabelChanged(const QString &appLabel);
  void appIconChanged(const QString &appIcon);
  void labelChanged(const QString &label);
  void iconChanged(const QString &icon);
  void sortIndexChanged(quint32 sortIndex);

private:
  void setIsCurrentView(bool isCurrentView);
  void createView();

  QuickEmbeddedShellWindow *m_embeddedShellWindow;
  bool m_isCurrentView;

  QString m_appId;
  QString m_appLabel;
  QString m_appIcon;
  QString m_label;
  QString m_icon;
  quint32 m_sortIndex;
  bool m_completed;
};

#endif // QUICKEMBEDDEDSHELLVIEW_H
