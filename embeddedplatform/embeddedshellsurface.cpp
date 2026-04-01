// SPDX-License-Identifier: LGPL-3.0-only

#include "embeddedshellsurface.h"
#include "embeddedshellsurface_p.h"
#include "embeddedshellsurfaceview.h"
#include "embeddedshellsurfaceview_p.h"
#include <QtWaylandClient/private/qwaylandwindow_p.h>

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

unsigned int EmbeddedShellSurface::sortIndex() const
{
  Q_D(const EmbeddedShellSurface);
  return d->m_sort_index;
}

void EmbeddedShellSurface::setSortIndex(unsigned int sortIndex)
{
  Q_D(EmbeddedShellSurface);

  if (d->m_sort_index != sortIndex) {
    d->m_sort_index = sortIndex;
    d->set_sort_index(sortIndex);
    emit sortIndexChanged(sortIndex);
  }
}

QString EmbeddedShellSurface::appId() const
{
  Q_D(const EmbeddedShellSurface);
  return d->m_app_id;
}

void EmbeddedShellSurface::setAppId(const QString &appId)
{
  Q_D(EmbeddedShellSurface);

  if (d->m_app_id != appId) {
    d->m_app_id = appId;
    d->set_app_id(appId);
    emit appIdChanged(appId);
  }
}

QString EmbeddedShellSurface::appLabel() const
{
  Q_D(const EmbeddedShellSurface);
  return d->m_app_label;
}

void EmbeddedShellSurface::setAppLabel(const QString &appLabel)
{
  Q_D(EmbeddedShellSurface);

  if (d->m_app_label != appLabel) {
    d->m_app_label = appLabel;
    d->set_app_label(appLabel);
    emit appLabelChanged(appLabel);
  }
}

QString EmbeddedShellSurface::appIcon() const
{
  Q_D(const EmbeddedShellSurface);
  return d->m_app_icon;
}

void EmbeddedShellSurface::setAppIcon(const QString &appIcon)
{
  Q_D(EmbeddedShellSurface);

  if (d->m_app_icon != appIcon) {
    d->m_app_icon = appIcon;
    d->set_app_icon(appIcon);
    emit appIconChanged(appIcon);
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

QVariant EmbeddedShellSurface::customData() const
{
  Q_D(const EmbeddedShellSurface);
  return d->m_custom_data;
}

void EmbeddedShellSurface::setCustomData(const QVariant &customData)
{
  Q_D(EmbeddedShellSurface);

  if (d->m_custom_data != customData) {
    d->m_custom_data = customData;
    d->set_custom_data(EmbeddedShellSurfaceViewPrivate::serializeVariant(customData));
    emit customDataChanged(customData);
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
                                                           uint32_t sortIndex)
{
  return createView(QString(), QString(), QString(), label, icon, sortIndex);
}

EmbeddedShellSurfaceView *EmbeddedShellSurface::createView(const QString &appId,
                                                           const QString &label,
                                                           const QString &icon,
                                                           uint32_t sortIndex)
{
  return createView(appId, QString(), QString(), label, icon, sortIndex);
}

EmbeddedShellSurfaceView *EmbeddedShellSurface::createView(const QString &appId,
                                                           const QString &appLabel,
                                                           const QString &appIcon,
                                                           const QString &label,
                                                           const QString &icon,
                                                           uint32_t sortIndex,
                                                           const QVariant &custom_data)
{
  Q_D(EmbeddedShellSurface);
  auto waylandView = d->view_create(d->embedded_shell_surface::object(),
                                    appId,
                                    appLabel,
                                    appIcon,
                                    label,
                                    icon,
                                    sortIndex,
                                    EmbeddedShellSurfaceViewPrivate::serializeVariant(custom_data));

  auto view = new EmbeddedShellSurfaceView(waylandView, this);

  auto *viewPrivate = EmbeddedShellSurfaceViewPrivate::get(view);

  viewPrivate->m_appId = appId;
  viewPrivate->m_appLabel = appLabel;
  viewPrivate->m_appIcon = appIcon;
  viewPrivate->m_label = label;
  viewPrivate->m_icon = icon;
  viewPrivate->m_sortIndex = sortIndex;
  viewPrivate->m_customData = custom_data;

  connect(view, &EmbeddedShellSurfaceView::selectedChanged, d, [view, d](bool selected) {
    if (selected) {
      if (d->m_selectedView != view) {
        if (d->m_selectedView) {
          d->m_selectedView->setSelected(false);
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
