// SPDX-License-Identifier: LGPL-3.0-only

#include "embeddedshellsurfaceview.h"
#include "embeddedshellsurfaceview_p.h"

EmbeddedShellSurfaceViewPrivate::EmbeddedShellSurfaceViewPrivate(EmbeddedShellSurfaceView *q,
                                                                 ::surface_view *view,
                                                                 EmbeddedShellSurface *surf)
    : QObject(surf)
    , QtWayland::surface_view(view)
    , q_ptr(q)
    , m_selected(false)
    , m_topLevel(false)
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
  Q_Q(EmbeddedShellSurfaceView);
  q->updateSelected(true, true);
}

QByteArray EmbeddedShellSurfaceViewPrivate::serializeVariantMap(const QVariantMap &variantMap)
{
  QByteArray byteArray;
  QDataStream stream(&byteArray, QDataStream::WriteOnly);
  stream.setVersion(QDataStream::Qt_6_8);
  stream << variantMap;

  if (stream.status() != QDataStream::Status::Ok)
  {
    qWarning() << Q_FUNC_INFO << "Could not serialize" << variantMap << "!";
    byteArray.clear();
  }

  return byteArray;
}

EmbeddedShellSurfaceView::EmbeddedShellSurfaceView(::surface_view *view, EmbeddedShellSurface *surface)
    : d_ptr(new EmbeddedShellSurfaceViewPrivate(this, view, surface))
{
  connect(this, &EmbeddedShellSurfaceView::selectedUpdated, this, &EmbeddedShellSurfaceView::selectedChanged);
}

EmbeddedShellSurfaceView *EmbeddedShellSurfaceView::parentView() const
{
  Q_D(const EmbeddedShellSurfaceView);
  return d->m_parentView;
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
  emit labelChanged(label);
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
  emit iconChanged(icon);
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
  emit sortIndexChanged(sortIndex);
}

QVariantMap EmbeddedShellSurfaceView::customData() const
{
  Q_D(const EmbeddedShellSurfaceView);
  return d->m_customData;
}

void EmbeddedShellSurfaceView::setCustomData(const QVariantMap &customData)
{
  Q_D(EmbeddedShellSurfaceView);
  if (d->m_customData == customData)
    return;
  d->m_customData = customData;
  d->set_custom_data(EmbeddedShellSurfaceViewPrivate::serializeVariantMap(customData));
  emit customDataChanged(customData);
}

bool EmbeddedShellSurfaceView::selected() const
{
  Q_D(const EmbeddedShellSurfaceView);
  return d->m_selected;
}

bool EmbeddedShellSurfaceView::topLevel() const
{
  Q_D(const EmbeddedShellSurfaceView);
  return d->m_topLevel;
}

void EmbeddedShellSurfaceView::select()
{
  Q_D(EmbeddedShellSurfaceView);
  d->select();
}

const surface_view *EmbeddedShellSurfaceView::view() const
{
  Q_D(const EmbeddedShellSurfaceView);
  return d->object();
}

void EmbeddedShellSurfaceView::updateSelected(bool selected, bool explicitly)
{
  Q_D(EmbeddedShellSurfaceView);
  if (d->m_selected == selected)
    return;

  d->m_selected = selected;
  emit selectedUpdated(selected, explicitly);
}

void EmbeddedShellSurfaceView::updateTopLevel(bool topLevel)
{
  Q_D(EmbeddedShellSurfaceView);
  if (d->m_topLevel == topLevel)
    return;
  d->m_topLevel = topLevel;
  emit topLevelChanged(topLevel);
}
