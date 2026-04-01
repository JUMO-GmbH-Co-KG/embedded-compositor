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
  Q_PROPERTY(QString appLabel READ appLabel WRITE setAppLabel NOTIFY appLabelChanged)
  Q_PROPERTY(QString appIcon READ appIcon WRITE setAppIcon NOTIFY appIconChanged)
  Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
  Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
  Q_PROPERTY(int sortIndex READ sortIndex WRITE setSortIndex NOTIFY sortIndexChanged)
  Q_PROPERTY(QVariant customData READ customData WRITE setCustomData NOTIFY customDataChanged)
  Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged)

public:
  QString appLabel() const;
  void setAppLabel(const QString &appLabel);

  QString appIcon() const;
  void setAppIcon(const QString &appIcon);

  QString label() const;
  void setLabel(const QString &label);

  QString icon() const;
  void setIcon(const QString &icon);

  unsigned int sortIndex() const;
  void setSortIndex(unsigned int sortIndex);

  QVariant customData() const;
  void setCustomData(const QVariant &customData);

  bool selected() const;
  void setSelected(bool selected);

signals:
  void appLabelChanged(const QString &appLabel);
  void appIconChanged(const QString &appIcon);
  void labelChanged(const QString &label);
  void iconChanged(const QString &icon);
  void sortIndexChanged(unsigned int sortIndex);
  void customDataChanged(const QVariant &customData);
  void selectedChanged(bool selected);

private:
  friend class EmbeddedShellSurface;
  EmbeddedShellSurfaceView(struct ::surface_view *view,
                           EmbeddedShellSurface *surf);
};
