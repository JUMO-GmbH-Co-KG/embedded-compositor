// SPDX-License-Identifier: LGPL-3.0-only

#include "embeddedshellsurfaceview.h"
#include "embeddedshellsurfaceview_p.h"

#include <QDebug>

EmbeddedShellSurfaceViewPrivate::EmbeddedShellSurfaceViewPrivate(EmbeddedShellSurfaceView *q,
                                                                 ::surface_view *view,
                                                                 EmbeddedShellSurface *surf)
    : QObject(surf)
    , QtWayland::surface_view(view)
    , q_ptr(q)
    , m_sortIndex(0)
    , m_selected(false)
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
  qDebug() << Q_FUNC_INFO;
  Q_Q(EmbeddedShellSurfaceView);
  q->setSelected(true);
}

QByteArray EmbeddedShellSurfaceViewPrivate::serializeVariant(const QVariant &variant)
{
  QByteArray byteArray;
  QDataStream stream(&byteArray, QDataStream::WriteOnly);
  stream.setVersion(QDataStream::Qt_6_8);
  stream << variant;

  if (stream.status() != QDataStream::Status::Ok)
  {
    qWarning() << Q_FUNC_INFO << "Could not serialize" << variant << "!";
    byteArray.clear();
  }

  return byteArray;
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
  emit appLabelChanged(appLabel);
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
  emit appIconChanged(appIcon);
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

QVariant EmbeddedShellSurfaceView::customData() const
{
  Q_D(const EmbeddedShellSurfaceView);
  return d->m_customData;
}

void EmbeddedShellSurfaceView::setCustomData(const QVariant &customData)
{
  Q_D(EmbeddedShellSurfaceView);
  if (d->m_customData == customData)
    return;
  d->m_customData = customData;
  d->set_custom_data(EmbeddedShellSurfaceViewPrivate::serializeVariant(customData));
  emit customDataChanged(customData);
}

bool EmbeddedShellSurfaceView::selected() const
{
  Q_D(const EmbeddedShellSurfaceView);
  return d->m_selected;
}

void EmbeddedShellSurfaceView::setSelected(bool selected)
{
  Q_D(EmbeddedShellSurfaceView);
  if (d->m_selected == selected)
    return;
  d->m_selected = selected;
  emit selectedChanged(selected);
}