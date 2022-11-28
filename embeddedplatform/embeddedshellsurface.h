// SPDX-License-Identifier: LGPL-3.0-only

#ifndef EMBEDDEDSHELLSURFACE_H
#define EMBEDDEDSHELLSURFACE_H

#include "embeddedplatform.h"
#include <QObject>

class EmbeddedShellSurfaceView;
class EmbeddedShellSurfaceViewPrivate;
class EmbeddedShellSurfacePrivate;

struct embedded_shell_surface;
struct surface_view;
class QWindow;

namespace QtWaylandClient {
class QWaylandWindow;
class QWaylandShellSurface;
} // namespace QtWaylandClient

class Q_DECL_EXPORT EmbeddedShellSurface : public QObject {
  Q_OBJECT
  Q_DECLARE_PRIVATE(EmbeddedShellSurface)
  QScopedPointer<EmbeddedShellSurfacePrivate> d_ptr;

public:
  EmbeddedShellSurface(struct ::embedded_shell_surface *shell_surface,
                       QtWaylandClient::QWaylandWindow *window,
                       EmbeddedShellTypes::Anchor anchor, uint32_t margin,
                       int32_t sort_index);
  ~EmbeddedShellSurface() override;

  EmbeddedShellTypes::Anchor getAnchor() const;
  int getSortIndex() const;
  EmbeddedShellSurfaceView *createView(const QString &label,
                                       int32_t sort_index);

  QtWaylandClient::QWaylandShellSurface *shellSurface();
signals:

public slots:
  void sendAnchor(EmbeddedShellTypes::Anchor anchor);
  void sendMargin(int margin);
  void sendSortIndex(int sortIndex);
};

class EmbeddedShellSurfaceView : public QObject {
  Q_OBJECT
  Q_DECLARE_PRIVATE(EmbeddedShellSurfaceView)
  QScopedPointer<EmbeddedShellSurfaceViewPrivate> d_ptr;
  Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)

public:
  EmbeddedShellSurfaceView(struct ::surface_view *view,
                           EmbeddedShellSurface *surf, const QString &label);
  ~EmbeddedShellSurfaceView() override;

  const QString &label() const;
  void setLabel(const QString &newLabel);

signals:
  void selected();
  void labelChanged();
};

#endif // EMBEDDEDSHELLSURFACE_H
