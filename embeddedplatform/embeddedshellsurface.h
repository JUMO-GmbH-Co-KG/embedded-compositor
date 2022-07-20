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
  using Anchor = EmbeddedPlatform::Anchor;

  EmbeddedShellSurface(struct ::embedded_shell_surface *shell_surface,
                       QtWaylandClient::QWaylandWindow *window, Anchor anchor);
  ~EmbeddedShellSurface() override;

  Anchor getAnchor() const;
  void applyConfigure();
  EmbeddedShellSurfaceView *createView();

  QtWaylandClient::QWaylandShellSurface *shellSurface();
signals:
  void viewCreated(EmbeddedShellSurfaceView *view);
  void viewSelected(EmbeddedShellSurfaceView *view);

public slots:
  void sendAnchor(Anchor anchor);
};

class EmbeddedShellSurfaceView : public QObject {
  Q_OBJECT
  Q_DECLARE_PRIVATE(EmbeddedShellSurfaceView)
  EmbeddedShellSurface *m_owningSurface;
  QScopedPointer<EmbeddedShellSurfaceViewPrivate> d_ptr;

public:
  EmbeddedShellSurfaceView(struct ::surface_view *view,
                           EmbeddedShellSurface *surf);
  ~EmbeddedShellSurfaceView() override;

signals:
  void selected();
};

#endif // EMBEDDEDSHELLSURFACE_H
