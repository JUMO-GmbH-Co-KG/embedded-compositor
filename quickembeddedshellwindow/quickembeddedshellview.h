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
  Q_PROPERTY(bool topLevel READ topLevel NOTIFY topLevelChanged)

  Q_PROPERTY(EmbeddedShellSurfaceView *view READ view NOTIFY viewChanged)
  Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
  Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
  Q_PROPERTY(quint32 sortIndex READ sortIndex WRITE setSortIndex NOTIFY sortIndexChanged)
  Q_PROPERTY(QVariantMap customData READ customData WRITE setCustomData NOTIFY customDataChanged)
  Q_PROPERTY(QuickEmbeddedShellView *parentView READ parentView WRITE setParentView NOTIFY parentViewChanged)

public:
  explicit QuickEmbeddedShellView(QQuickItem *parent = nullptr);

  void componentComplete() override;

  QuickEmbeddedShellSurface *surface() const;
  void setSurface(QuickEmbeddedShellSurface *surface);

  bool selected() const;

  bool topLevel() const;

  EmbeddedShellSurfaceView *view() const;

  QString label() const;
  void setLabel(const QString &label);

  QString icon() const;
  void setIcon(const QString &icon);

  quint32 sortIndex() const;
  void setSortIndex(quint32 sortIndex);

  QVariantMap customData() const;
  void setCustomData(const QVariantMap &customData);

  QuickEmbeddedShellView *parentView() const;
  void setParentView(QuickEmbeddedShellView *parentView);

signals:
  void select();
  void surfaceChanged();
  void selectedChanged();
  void topLevelChanged();
  void viewChanged();
  void parentViewChanged();
  void labelChanged();
  void iconChanged();
  void sortIndexChanged();
  void customDataChanged();

private:
  void updateSelected(bool selected);
  void updateTopLevel(bool topLevel);
  void createView();

  QuickEmbeddedShellSurface *m_surface;
  bool m_selected;
  bool m_topLevel;

  EmbeddedShellSurfaceView *m_view;
  QuickEmbeddedShellView *m_parentView;
  QString m_label;
  QString m_icon;
  quint32 m_sortIndex;
  QVariantMap m_customData;
  bool m_completed;
};
