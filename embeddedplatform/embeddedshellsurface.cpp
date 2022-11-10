#include "embeddedshellsurface.h"
#include "embeddedshellsurface_p.h"
#include <QtWaylandClient/private/qwaylandwindow_p.h>
#include <qpa/qplatformwindow.h>
#include <qpa/qwindowsysteminterface.h>

EmbeddedShellSurface::EmbeddedShellSurface(
    struct ::embedded_shell_surface *shell_surface, QWaylandWindow *window,
    EmbeddedShellTypes::Anchor anchor, uint32_t margin, int32_t sort_index)
    : d_ptr(new EmbeddedShellSurfacePrivate(shell_surface, window, anchor,
                                            margin, sort_index)) {
  qDebug() << __PRETTY_FUNCTION__ << anchor;
}

EmbeddedShellSurface::~EmbeddedShellSurface() {}

EmbeddedShellSurfacePrivate::EmbeddedShellSurfacePrivate(
    struct ::embedded_shell_surface *shell_surface,
    QtWaylandClient::QWaylandWindow *window, EmbeddedShellTypes::Anchor anchor,
    uint32_t margin, int32_t sort_index)
    : QWaylandShellSurface(window), QtWayland::embedded_shell_surface(
                                        shell_surface),
      m_anchor(anchor), m_margin(margin), m_sort_index(sort_index) {
  qDebug() << __PRETTY_FUNCTION__ << anchor << margin;
}

EmbeddedShellSurfacePrivate::~EmbeddedShellSurfacePrivate() {}

EmbeddedShellTypes::Anchor EmbeddedShellSurface::getAnchor() const {
  Q_D(const EmbeddedShellSurface);
  return d->m_anchor;
}

int EmbeddedShellSurface::getSortIndex() const {
  Q_D(const EmbeddedShellSurface);
  return d->m_sort_index;
}

void EmbeddedShellSurfacePrivate::applyConfigure() {
  qDebug() << __PRETTY_FUNCTION__ << m_pendingSize;
  window()->resizeFromApplyConfigure(m_pendingSize);
}

void EmbeddedShellSurfacePrivate::embedded_shell_surface_configure(
    int32_t width, int32_t height) {
  qDebug() << __PRETTY_FUNCTION__ << width << height;
  m_pendingSize = {width, height};
  window()->applyConfigureWhenPossible();
}

EmbeddedShellSurfaceView *EmbeddedShellSurface::createView(const QString &label,
                                                           int32_t sort_index) {
  Q_D(EmbeddedShellSurface);
  auto view =
      d->view_create(d->embedded_shell_surface::object(), label, sort_index);
  auto ret = new EmbeddedShellSurfaceView(view, this, label);
  return ret;
}

QWaylandShellSurface *EmbeddedShellSurface::shellSurface() {
  return d_ptr.data();
}

void EmbeddedShellSurface::sendAnchor(EmbeddedShellTypes::Anchor anchor) {
  qDebug() << __PRETTY_FUNCTION__ << anchor;
  Q_D(EmbeddedShellSurface);
  d->set_anchor(static_cast<embedded_shell_anchor_border>(anchor));
}

void EmbeddedShellSurface::sendMargin(int margin) {
  qDebug() << __PRETTY_FUNCTION__ << margin;
  Q_D(EmbeddedShellSurface);
  d->set_margin(margin);
}

void EmbeddedShellSurface::sendSortIndex(int sortIndex) {
  qDebug() << __PRETTY_FUNCTION__ << sortIndex;
  Q_D(EmbeddedShellSurface);
  d->set_sort_index(sortIndex);
}

EmbeddedShellSurfaceViewPrivate::EmbeddedShellSurfaceViewPrivate(
    EmbeddedShellSurfaceView *q, ::surface_view *view,
    EmbeddedShellSurface *surf, const QString &label)
    : QObject(surf), QtWayland::surface_view(view), m_label(label), q_ptr(q) {
  qDebug() << __PRETTY_FUNCTION__;
}

EmbeddedShellSurfaceView::EmbeddedShellSurfaceView(::surface_view *view,
                                                   EmbeddedShellSurface *surf,
                                                   const QString &label)
    : d_ptr(new EmbeddedShellSurfaceViewPrivate(this, view, surf, label)) {}

EmbeddedShellSurfaceView::~EmbeddedShellSurfaceView() {
  qDebug() << __PRETTY_FUNCTION__;
}

const QString &EmbeddedShellSurfaceView::label() const {
  Q_D(const EmbeddedShellSurfaceView);
  return d->m_label;
}

void EmbeddedShellSurfaceView::setLabel(const QString &newLabel) {
  Q_D(EmbeddedShellSurfaceView);
  if (d->m_label == newLabel)
    return;
  d->m_label = newLabel;
  emit labelChanged();
}
