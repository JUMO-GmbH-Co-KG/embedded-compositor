// SPDX-License-Identifier: LGPL-3.0-only

#ifndef EMBEDDEDSHELLSURFACE_H
#define EMBEDDEDSHELLSURFACE_H

#include "embeddedshelltypes.h"

#include <QObject>

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

public:
  EmbeddedShellSurface(struct ::embedded_shell_surface *shell_surface,
                       QtWaylandClient::QWaylandWindow *window,
                       const QSize &size,
                       EmbeddedShellTypes::Anchor anchor, uint32_t margin,
                       uint32_t sort_index);

  QSize getSize() const;
  EmbeddedShellTypes::Anchor getAnchor() const;
  unsigned int getSortIndex() const;
  bool getVisible();

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
                                       uint32_t sort_index);

  QtWaylandClient::QWaylandShellSurface *shellSurface();

signals:
  void visibleChanged(bool visible);

public slots:
  void sendSize(const QSize &size);
  void sendAnchor(EmbeddedShellTypes::Anchor anchor);
  void sendMargin(int margin);
  void sendSortIndex(unsigned int sortIndex);
  void sendAppId(const QString &appId);
  void sendAppLabel(const QString &appLabe);
  void sendAppIcon(const QString &appIcon);
};

#endif // EMBEDDEDSHELLSURFACE_H
