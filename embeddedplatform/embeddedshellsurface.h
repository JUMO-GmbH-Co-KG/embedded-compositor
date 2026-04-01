// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "embeddedshelltypes.h"

#include <QObject>
#include <QVariant>

class EmbeddedShellSurfaceView;
class EmbeddedShellSurfacePrivate;

struct embedded_shell_surface;

namespace QtWaylandClient {
  class QWaylandWindow;
  class QWaylandShellSurface;
} // namespace QtWaylandClient

class Q_DECL_EXPORT EmbeddedShellSurface : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(EmbeddedShellSurface)
  QScopedPointer<EmbeddedShellSurfacePrivate> d_ptr;
  Q_PROPERTY(EmbeddedShellTypes::Anchor anchor READ anchor WRITE setAnchor NOTIFY anchorChanged)
  Q_PROPERTY(int margin READ margin WRITE setMargin NOTIFY marginChanged)
  Q_PROPERTY(unsigned int sortIndex READ sortIndex WRITE setSortIndex NOTIFY sortIndexChanged)
  Q_PROPERTY(QString appId READ appId WRITE setAppId NOTIFY appIdChanged)
  Q_PROPERTY(QString appLabel READ appLabel WRITE setAppLabel NOTIFY appLabelChanged)
  Q_PROPERTY(QString appIcon READ appIcon WRITE setAppIcon NOTIFY appIconChanged)
  Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged)
  Q_PROPERTY(QVariant customData READ customData WRITE setCustomData NOTIFY customDataChanged)

  Q_PROPERTY(bool visible READ visible NOTIFY visibleChanged)

public:
  EmbeddedShellSurface(struct ::embedded_shell_surface *shell_surface,
                       QtWaylandClient::QWaylandWindow *window,
                       const QSize &size,
                       EmbeddedShellTypes::Anchor anchor,
                       uint32_t margin,
                       uint32_t sort_index);

public:
  EmbeddedShellTypes::Anchor anchor() const;
  void setAnchor(EmbeddedShellTypes::Anchor anchor);

  int margin() const;
  void setMargin(int margin);

  unsigned int sortIndex() const;
  void setSortIndex(unsigned int sortIndex);

  QString appId() const;
  void setAppId(const QString &appId);

  QString appLabel() const;
  void setAppLabel(const QString &appLabel);

  QString appIcon() const;
  void setAppIcon(const QString &appIcon);

  QSize size() const;
  void setSize(const QSize &size);

  QVariant customData() const;
  void setCustomData(const QVariant &customData);

  bool visible() const;

  EmbeddedShellSurfaceView *createView(const QString &label,
                                       const QString &icon,
                                       uint32_t sort_index);

  EmbeddedShellSurfaceView *createView(const QString &appId,
                                       const QString &label,
                                       const QString &icon,
                                       uint32_t sort_index);

  EmbeddedShellSurfaceView *createView(const QString &appId,
                                       const QString &appLabel,
                                       const QString &appIcon,
                                       const QString &label,
                                       const QString &icon,
                                       uint32_t sort_index,
                                       const QVariant &custom_data = QVariant());

  QtWaylandClient::QWaylandShellSurface *shellSurface();

signals:
  void anchorChanged(EmbeddedShellTypes::Anchor anchor);
  void marginChanged(int margin);
  void sortIndexChanged(unsigned int sortIndex);
  void appIdChanged(const QString &appId);
  void appLabelChanged(const QString &appLabel);
  void appIconChanged(const QString &appIcon);
  void sizeChanged(const QSize &size);
  void customDataChanged(const QVariant &customData);
  void visibleChanged(bool visible);
};
