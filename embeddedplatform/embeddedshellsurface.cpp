// SPDX-License-Identifier: LGPL-3.0-only

#include "embeddedshellsurface.h"
#include "embeddedshellsurface_p.h"
#include <QtWaylandClient/private/qwaylandwindow_p.h>
#include <qpa/qplatformwindow.h>
#include <qpa/qwindowsysteminterface.h>

EmbeddedShellSurface::EmbeddedShellSurface(struct ::embedded_shell_surface *shell_surface,
                                           QtWaylandClient::QWaylandWindow *window,
                                           const QSize &size,
                                           EmbeddedShellTypes::Anchor anchor,
                                           uint32_t margin,
                                           uint32_t sort_index)
    : d_ptr(new EmbeddedShellSurfacePrivate(this,
                                            shell_surface,
                                            window,
                                            size,
                                            anchor,
                                            margin,
                                            sort_index))
{
}

EmbeddedShellSurfacePrivate::EmbeddedShellSurfacePrivate(EmbeddedShellSurface *q,
                                                         struct ::embedded_shell_surface *shell_surface,
                                                         QtWaylandClient::QWaylandWindow *window,
                                                         const QSize &size,
                                                         EmbeddedShellTypes::Anchor anchor,
                                                         uint32_t margin,
                                                         uint32_t sort_index)
    : QWaylandShellSurface(window)
    , QtWayland::embedded_shell_surface(shell_surface)
    , m_size(size)
    , m_anchor(anchor)
    , m_margin(margin)
    , m_sort_index(sort_index)
    , q_ptr(q)
{
}

EmbeddedShellSurfacePrivate::~EmbeddedShellSurfacePrivate()
{
}

QSize EmbeddedShellSurface::getSize() const
{
  Q_D(const EmbeddedShellSurface);
  return d->m_size;
}

EmbeddedShellTypes::Anchor EmbeddedShellSurface::getAnchor() const
{
  Q_D(const EmbeddedShellSurface);
  return d->m_anchor;
}

unsigned int EmbeddedShellSurface::getSortIndex() const
{
  Q_D(const EmbeddedShellSurface);
  return d->m_sort_index;
}

bool EmbeddedShellSurface::getVisible()
{
  Q_D(const EmbeddedShellSurface);
  return d->m_visible;
}

void EmbeddedShellSurfacePrivate::applyConfigure()
{
  window()->resizeFromApplyConfigure(m_pendingSize);
}

void EmbeddedShellSurfacePrivate::embedded_shell_surface_configure(int32_t width,
                                                                   int32_t height)
{
  m_pendingSize = {width, height};
  window()->applyConfigureWhenPossible();
}

void EmbeddedShellSurfacePrivate::embedded_shell_surface_visible_changed(int32_t visible)
{
  Q_Q(EmbeddedShellSurface);
  auto bVisible = static_cast<bool>(visible);
  if (m_visible != bVisible) {
    m_visible = bVisible;
    emit q->visibleChanged(bVisible);
  }
}

EmbeddedShellSurfaceView *EmbeddedShellSurface::createView(const QString &label,
                                                           const QString &icon,
                                                           uint32_t sort_index)
{
  return createView(QString(), QString(), QString(), label, icon, sort_index);
}

EmbeddedShellSurfaceView *EmbeddedShellSurface::createView(const QString &appId,
                                                           const QString &label,
                                                           const QString &icon,
                                                           uint32_t sort_index)
{
  return createView(appId, QString(), QString(), label, icon, sort_index);
}

EmbeddedShellSurfaceView *EmbeddedShellSurface::createView(const QString &appId,
                                                           const QString &appLabel,
                                                           const QString &appIcon,
                                                           const QString &label,
                                                           const QString &icon,
                                                           uint32_t sort_index)
{
  Q_D(EmbeddedShellSurface);
  auto waylandView = d->view_create(d->embedded_shell_surface::object(),
                             appId,
                             appLabel,
                             appIcon,
                             label,
                             icon,
                             sort_index);

  auto view = new EmbeddedShellSurfaceView(waylandView, this);

  auto *viewPrivate = EmbeddedShellSurfaceViewPrivate::get(view);

  viewPrivate->m_appId = appId;
  viewPrivate->m_appLabel = appLabel;
  viewPrivate->m_appIcon = appIcon;
  viewPrivate->m_label = label;
  viewPrivate->m_icon = icon;
  viewPrivate->m_sortIndex = sort_index;

  connect(view, &EmbeddedShellSurfaceView::selected, d, [view, d]() {
    if (d->m_selectedView != view) {
      if (d->m_selectedView) {
        d->m_selectedView->deselected();
      }

      d->m_selectedView = view;
    }
  });

  return view;
}

QtWaylandClient::QWaylandShellSurface *EmbeddedShellSurface::shellSurface()
{
  return d_ptr.data();
}

void EmbeddedShellSurface::sendSize(const QSize &size)
{
  Q_D(EmbeddedShellSurface);
  // Check version here if it were versioned.
  d->set_size(size.width(), size.height());
}

void EmbeddedShellSurface::sendAnchor(EmbeddedShellTypes::Anchor anchor)
{
  Q_D(EmbeddedShellSurface);
  d->set_anchor(static_cast<embedded_shell_anchor_border>(anchor));
}

void EmbeddedShellSurface::sendMargin(int margin)
{
  Q_D(EmbeddedShellSurface);
  d->set_margin(margin);
}

void EmbeddedShellSurface::sendSortIndex(unsigned int sortIndex)
{
  Q_D(EmbeddedShellSurface);
  d->set_sort_index(sortIndex);
}

void EmbeddedShellSurface::sendAppId(const QString &appId)
{
  Q_D(EmbeddedShellSurface);
  d->set_app_id(appId);
}

void EmbeddedShellSurface::sendAppLabel(const QString &appLabel)
{
  Q_D(EmbeddedShellSurface);
  d->set_app_label(appLabel);
}

void EmbeddedShellSurface::sendAppIcon(const QString &appIcon)
{
  Q_D(EmbeddedShellSurface);
  d->set_app_icon(appIcon);
}

EmbeddedShellSurfaceViewPrivate::EmbeddedShellSurfaceViewPrivate(EmbeddedShellSurfaceView *q,
                                                                 ::surface_view *view,
                                                                 EmbeddedShellSurface *surf)
    : QObject(surf)
    , QtWayland::surface_view(view)
    , q_ptr(q)
    , m_sortIndex(0)
{
}

EmbeddedShellSurfaceViewPrivate::~EmbeddedShellSurfaceViewPrivate()
{
  destroy();
}

EmbeddedShellSurfaceViewPrivate *EmbeddedShellSurfaceViewPrivate::get(EmbeddedShellSurfaceView *q)
{
  return q->d_func();
}

void EmbeddedShellSurfaceViewPrivate::surface_view_selected()
{
  qDebug() << __PRETTY_FUNCTION__;
  Q_Q(EmbeddedShellSurfaceView);
  emit q->selected();
}

EmbeddedShellSurfaceView::EmbeddedShellSurfaceView(::surface_view *view,
                                                   EmbeddedShellSurface *surf)
    : d_ptr(new EmbeddedShellSurfaceViewPrivate(this, view, surf))
{
}

QString EmbeddedShellSurfaceView::appLabel() const
{
  Q_D(const EmbeddedShellSurfaceView);
  return d->m_appLabel;
}

void EmbeddedShellSurfaceView::setAppLabel(const QString &appLabel)
{
  Q_D(EmbeddedShellSurfaceView);
  if (d->m_appLabel == appLabel)
    return;
  d->m_appLabel = appLabel;
  d->set_app_label(appLabel);
  Q_EMIT appLabelChanged(appLabel);
}

QString EmbeddedShellSurfaceView::appIcon() const
{
  Q_D(const EmbeddedShellSurfaceView);
  return d->m_appIcon;
}

void EmbeddedShellSurfaceView::setAppIcon(const QString &appIcon)
{
  Q_D(EmbeddedShellSurfaceView);
  if (d->m_appIcon == appIcon)
    return;
  d->m_appIcon = appIcon;
  d->set_app_icon(appIcon);
  Q_EMIT appIconChanged(appIcon);
}

QString EmbeddedShellSurfaceView::label() const
{
  Q_D(const EmbeddedShellSurfaceView);
  return d->m_label;
}

void EmbeddedShellSurfaceView::setLabel(const QString &label)
{
  Q_D(EmbeddedShellSurfaceView);
  if (d->m_label == label)
    return;
  d->m_label = label;
  d->set_label(label);
  Q_EMIT labelChanged(label);
}

QString EmbeddedShellSurfaceView::icon() const
{
  Q_D(const EmbeddedShellSurfaceView);
  return d->m_icon;
}

void EmbeddedShellSurfaceView::setIcon(const QString &icon)
{
  Q_D(EmbeddedShellSurfaceView);
  if (d->m_icon == icon)
    return;
  d->m_icon = icon;
  d->set_icon(icon);
  Q_EMIT iconChanged(icon);
}

unsigned int EmbeddedShellSurfaceView::sortIndex() const
{
  Q_D(const EmbeddedShellSurfaceView);
  return d->m_sortIndex;
}

void EmbeddedShellSurfaceView::setSortIndex(unsigned int sortIndex)
{
  Q_D(EmbeddedShellSurfaceView);
  if (d->m_sortIndex == sortIndex)
    return;
  d->m_sortIndex = sortIndex;
  d->set_sort_index(sortIndex);
  Q_EMIT sortIndexChanged(sortIndex);
}
