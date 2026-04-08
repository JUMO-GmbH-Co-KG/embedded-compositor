// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "embeddedshellsurface.h"

#include <QObject>

class EmbeddedShellSurfaceViewPrivate;

struct surface_view;

class EmbeddedShellSurfaceView : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(EmbeddedShellSurfaceView)
  QScopedPointer<EmbeddedShellSurfaceViewPrivate> d_ptr;
  Q_PROPERTY(EmbeddedShellSurfaceView *parentView READ parentView CONSTANT)
  Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
  Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
  Q_PROPERTY(int sortIndex READ sortIndex WRITE setSortIndex NOTIFY sortIndexChanged)
  Q_PROPERTY(QVariant customData READ customData WRITE setCustomData NOTIFY customDataChanged)
  Q_PROPERTY(bool selected READ selected NOTIFY selectedChanged)
  Q_PROPERTY(bool topLevel READ topLevel NOTIFY topLevelChanged)

public:
  EmbeddedShellSurfaceView *parentView() const;

  QString label() const;
  void setLabel(const QString &label);

  QString icon() const;
  void setIcon(const QString &icon);

  unsigned int sortIndex() const;
  void setSortIndex(unsigned int sortIndex);

  QVariant customData() const;
  void setCustomData(const QVariant &customData);

  bool selected() const;

  bool topLevel() const;

  void select();

  const ::surface_view *view() const;

signals:
  void labelChanged(const QString &label);
  void iconChanged(const QString &icon);
  void sortIndexChanged(unsigned int sortIndex);
  void customDataChanged(const QVariant &customData);
  void selectedUpdated(bool selected, bool explicitly = false);
  void selectedChanged(bool selected);
  void topLevelChanged(bool topLevel);

private:
  void updateSelected(bool selected, bool explicitly = false);
  void updateTopLevel(bool topLevel);

private:
  friend class EmbeddedShellSurface;
  EmbeddedShellSurfaceView(struct ::surface_view *view, EmbeddedShellSurface *surface);
};
