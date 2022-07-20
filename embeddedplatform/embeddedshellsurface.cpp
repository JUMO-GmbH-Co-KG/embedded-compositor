#include "embeddedshellsurface.h"
#include "embeddedshellsurface_p.h"
#include <QtWaylandClient/private/qwaylandwindow_p.h>
#include <qpa/qplatformwindow.h>
#include <qpa/qwindowsysteminterface.h>

using namespace QtWaylandClient;

EmbeddedShellSurface::EmbeddedShellSurface(
    struct ::embedded_shell_surface *shell_surface, QWaylandWindow *window,
    Anchor anchor)
    : d_ptr(new EmbeddedShellSurfacePrivate(shell_surface, window, anchor)) {
  qDebug() << __PRETTY_FUNCTION__ << anchor;
}

EmbeddedShellSurface::~EmbeddedShellSurface() {}

EmbeddedShellSurfacePrivate::EmbeddedShellSurfacePrivate(
    struct ::embedded_shell_surface *shell_surface,
    QtWaylandClient::QWaylandWindow *window, Anchor anchor)
    : QWaylandShellSurface(window), QtWayland::embedded_shell_surface(
                                        shell_surface),
      m_anchor(anchor) {
  qDebug() << __PRETTY_FUNCTION__ << anchor;
}

EmbeddedShellSurfacePrivate::~EmbeddedShellSurfacePrivate() {}

EmbeddedShellSurface::Anchor EmbeddedShellSurface::getAnchor() const {
  Q_D(const EmbeddedShellSurface);
  return d->m_anchor;
}

void EmbeddedShellSurface::applyConfigure() {
  Q_D(EmbeddedShellSurface);
  d->window()->resizeFromApplyConfigure(d->m_pendingSize);
}

void EmbeddedShellSurfacePrivate::embedded_shell_surface_configure(
    int32_t width, int32_t height) {
  qDebug() << __PRETTY_FUNCTION__ << width << height;
  m_pendingSize = {width, height};
  window()->applyConfigureWhenPossible();
}

EmbeddedShellSurfaceView *EmbeddedShellSurface::createView() {
  Q_D(EmbeddedShellSurface);
  auto view = d->view_create(d->embedded_shell_surface::object());
  auto ret = new EmbeddedShellSurfaceView(view, this);
  return ret;
}

QWaylandShellSurface *EmbeddedShellSurface::shellSurface() {
  return d_ptr.data();
}

void EmbeddedShellSurface::sendAnchor(Anchor anchor) {
  qDebug() << __PRETTY_FUNCTION__ << anchor;
  Q_D(EmbeddedShellSurface);
  d->set_anchor(static_cast<embedded_shell_anchor_border>(anchor));
}

EmbeddedShellSurfaceViewPrivate::EmbeddedShellSurfaceViewPrivate(
    ::surface_view *view, EmbeddedShellSurface *surf)
    : QObject(surf), QtWayland::surface_view(view), m_owningSurface(surf) {
  qDebug() << __PRETTY_FUNCTION__;
}

EmbeddedShellSurfaceView::EmbeddedShellSurfaceView(::surface_view *view,
                                                   EmbeddedShellSurface *surf)
    : d_ptr(new EmbeddedShellSurfaceViewPrivate(view, surf)) {}

EmbeddedShellSurfaceView::~EmbeddedShellSurfaceView() {
  qDebug() << __PRETTY_FUNCTION__;
}
