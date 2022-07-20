#ifndef EMBEDDEDSHELLSURFACE_P_H
#define EMBEDDEDSHELLSURFACE_P_H
#include "embeddedshellsurface.h"
#include "qwayland-embedded-shell.h"
#include <QDebug>
#include <QtWaylandClient/private/qwaylandshellsurface_p.h>

class EmbeddedShellSurfacePrivate
    : public QtWaylandClient::QWaylandShellSurface,
      public QtWayland::embedded_shell_surface {
  Q_DECLARE_PUBLIC(EmbeddedShellSurface)
  Q_OBJECT
public:
  using Anchor = EmbeddedShellSurface::Anchor;
  EmbeddedShellSurfacePrivate(struct ::embedded_shell_surface *shell_surface,
                              QtWaylandClient::QWaylandWindow *window,
                              Anchor anchor);
  ~EmbeddedShellSurfacePrivate() override;
  Anchor getAnchor() const { return m_anchor; }

private:
  EmbeddedShellSurface::Anchor m_anchor;
  QSize m_pendingSize = {0, 0};
  //    QtWaylandClient::QWaylandWindow *m_window = nullptr;
  EmbeddedShellSurface *q_ptr = nullptr;

protected:
  void embedded_shell_surface_configure(int32_t width, int32_t height) override;
};

class EmbeddedShellSurfaceViewPrivate : public QObject,
                                        public QtWayland::surface_view {
  Q_DECLARE_PUBLIC(EmbeddedShellSurfaceView)
  Q_OBJECT
public:
  EmbeddedShellSurfaceViewPrivate(::surface_view *view,
                                  EmbeddedShellSurface *surf);
  void surface_view_selected() override {
    Q_Q(EmbeddedShellSurfaceView);
    emit q->selected();
  }
  EmbeddedShellSurface *m_owningSurface = nullptr;
  EmbeddedShellSurfaceView *q_ptr = nullptr;
};
#endif // EMBEDDEDSHELLSURFACE_P_H
