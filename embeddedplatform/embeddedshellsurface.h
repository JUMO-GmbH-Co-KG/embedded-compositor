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
  Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged)

  Q_PROPERTY(bool visible READ visible NOTIFY visibleChanged)

public:
  EmbeddedShellSurface(struct ::embedded_shell_surface *shellSurface,
                       QtWaylandClient::QWaylandWindow *window,
                       const QSize &size,
                       EmbeddedShellTypes::Anchor anchor,
                       uint32_t margin);

public:
  EmbeddedShellTypes::Anchor anchor() const;
  void setAnchor(EmbeddedShellTypes::Anchor anchor);

  int margin() const;
  void setMargin(int margin);

  QSize size() const;
  void setSize(const QSize &size);

  bool visible() const;

  EmbeddedShellSurfaceView *createView(const QString &label,
                                       const QString &icon,
                                       uint32_t sortIndex,
                                       const QVariant &customData = QVariant(),
                                       EmbeddedShellSurfaceView* parentView = nullptr);

  QtWaylandClient::QWaylandShellSurface *shellSurface();

signals:
  void anchorChanged(EmbeddedShellTypes::Anchor anchor);
  void marginChanged(int margin);
  void sizeChanged(const QSize &size);
  void visibleChanged(bool visible);
};
