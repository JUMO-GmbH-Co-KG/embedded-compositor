// SPDX-License-Identifier: LGPL-3.0-only

#include "embeddedshellsurface.h"
#include "embeddedshellsurface_p.h"
#include "embeddedshellsurfaceview.h"
#include "embeddedshellsurfaceview_p.h"
#include <QtWaylandClient/private/qwaylandwindow_p.h>

EmbeddedShellSurface::EmbeddedShellSurface(struct ::embedded_shell_surface *shellSurface,
                                           QtWaylandClient::QWaylandWindow *window,
                                           const QSize &size,
                                           EmbeddedShellTypes::Anchor anchor,
                                           uint32_t margin)
    : d_ptr(new EmbeddedShellSurfacePrivate(this,
                                            shellSurface,
                                            window,
                                            size,
                                            anchor,
                                            margin))
{
}

EmbeddedShellSurfacePrivate::EmbeddedShellSurfacePrivate(EmbeddedShellSurface *q,
                                                         struct ::embedded_shell_surface *shell_surface,
                                                         QtWaylandClient::QWaylandWindow *window,
                                                         const QSize &size,
                                                         EmbeddedShellTypes::Anchor anchor,
                                                         uint32_t margin)
    : QWaylandShellSurface(window)
    , QtWayland::embedded_shell_surface(shell_surface)
    , m_size(size)
    , m_anchor(anchor)
    , m_margin(margin)
    , q_ptr(q)
{
}

EmbeddedShellSurfacePrivate::~EmbeddedShellSurfacePrivate()
{
}

EmbeddedShellTypes::Anchor EmbeddedShellSurface::anchor() const
{
  Q_D(const EmbeddedShellSurface);
  return d->m_anchor;
}

void EmbeddedShellSurface::setAnchor(EmbeddedShellTypes::Anchor anchor)
{
  Q_D(EmbeddedShellSurface);
  if (d->m_anchor != anchor) {
    d->m_anchor = anchor;
    d->set_anchor(static_cast<embedded_shell_anchor_border>(anchor));
    emit anchorChanged(anchor);
  }
}

int EmbeddedShellSurface::margin() const
{
  Q_D(const EmbeddedShellSurface);
  return d->m_margin;
}

void EmbeddedShellSurface::setMargin(int margin)
{
  Q_D(EmbeddedShellSurface);

  if (d->m_margin != margin) {
    d->m_margin = margin;
    d->set_margin(margin);
    emit marginChanged(margin);
  }
}

QSize EmbeddedShellSurface::size() const
{
  Q_D(const EmbeddedShellSurface);
  return d->m_size;
}

void EmbeddedShellSurface::setSize(const QSize &size)
{
  Q_D(EmbeddedShellSurface);

  if (d->m_size != size) {
    d->m_size = size;
    d->set_size(size.width(), size.height());
    emit sizeChanged(size);
  }
}

bool EmbeddedShellSurface::visible() const
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
                                                           uint32_t sortIndex,
                                                           const QVariant &customData,
                                                           EmbeddedShellSurfaceView* parentView)
{
  Q_D(EmbeddedShellSurface);
  auto waylandView = d->view_create(d->embedded_shell_surface::object(),
                                    label,
                                    icon,
                                    sortIndex,
                                    EmbeddedShellSurfaceViewPrivate::serializeVariant(customData),
                                    parentView ? const_cast<::surface_view *>(parentView->view()) : nullptr);

  auto view = new EmbeddedShellSurfaceView(waylandView, this);

  auto *viewPrivate = EmbeddedShellSurfaceViewPrivate::get(view);

  viewPrivate->m_parentView = parentView;
  viewPrivate->m_label = label;
  viewPrivate->m_icon = icon;
  viewPrivate->m_sortIndex = sortIndex;
  viewPrivate->m_customData = customData;

  connect(view, &EmbeddedShellSurfaceView::selectedUpdated, d, [view, d](bool selected, bool explicitly) {
    if (selected && explicitly) {
      if (d->m_selectedView != view) {

        if (d->m_selectedView) {
          d->m_selectedView->updateTopLevel(false);
        }

        view->updateTopLevel(true);

        auto selectedViews = QList<EmbeddedShellSurfaceView *>() << view;
        auto selectedView = view->parentView();

        while (selectedView)
        {
          selectedView->updateSelected(true);
          selectedViews << selectedView;
          selectedView = selectedView->parentView();
        }

        EmbeddedShellSurfaceView *previousView = d->m_selectedView;
        while (previousView) {
          if (!selectedViews.contains(previousView))
          {
            previousView->updateSelected(false);
          }
          previousView = previousView->parentView();
        }

        d->m_selectedView = view;
      }
    }
  });

  return view;
}

QtWaylandClient::QWaylandShellSurface *EmbeddedShellSurface::shellSurface()
{
  return d_ptr.data();
}
