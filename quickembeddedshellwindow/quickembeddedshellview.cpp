#include "quickembeddedshellview.h"
#include "embeddedshellsurface.h"

QuickEmbeddedShellView::QuickEmbeddedShellView(QQuickItem *parent)
    : QQuickItem(parent)
    , m_surface(nullptr)
    , m_selected(false)
    , m_sortIndex(0)
    , m_completed(false)
{
}

void QuickEmbeddedShellView::componentComplete()
{
  Q_ASSERT(m_surface);

  if (m_surface->completed()) {
    createView();
  } else {
    connect(m_surface, &QuickEmbeddedShellSurface::completedChanged, this, &QuickEmbeddedShellView::createView, Qt::SingleShotConnection);
  }

  QQuickItem::componentComplete();
}

QuickEmbeddedShellSurface *QuickEmbeddedShellView::surface() const
{
  return m_surface;
}

void QuickEmbeddedShellView::setSurface(QuickEmbeddedShellSurface *surface)
{
  if (!m_surface && surface) {
    m_surface = surface;
    emit surfaceChanged(surface);
  }
}

bool QuickEmbeddedShellView::selected() const
{
  return m_selected;
}

void QuickEmbeddedShellView::setSelected(bool selected)
{
  if (m_selected != selected) {
    m_selected = selected;
    emit selectedChanged(selected);
  }
}

void QuickEmbeddedShellView::createView()
{
  auto view = m_surface->createView(m_appId, m_appLabel, m_appIcon, m_label, m_icon, m_sortIndex, m_customData);
  Q_ASSERT(view);

  connect(view, &EmbeddedShellSurfaceView::selectedChanged, this, &QuickEmbeddedShellView::setSelected);
  connect(this, &QuickEmbeddedShellView::appLabelChanged, view, &EmbeddedShellSurfaceView::setAppLabel);
  connect(this, &QuickEmbeddedShellView::appIconChanged, view, &EmbeddedShellSurfaceView::setAppIcon);
  connect(this, &QuickEmbeddedShellView::labelChanged, view, &EmbeddedShellSurfaceView::setLabel);
  connect(this, &QuickEmbeddedShellView::iconChanged, view, &EmbeddedShellSurfaceView::setIcon);
  connect(this, &QuickEmbeddedShellView::sortIndexChanged, view, &EmbeddedShellSurfaceView::setSortIndex);
  connect(this, &QuickEmbeddedShellView::customDataChanged, view, &EmbeddedShellSurfaceView::setCustomData);
  connect(this, &QObject::destroyed, view, &QObject::deleteLater);

  m_completed = true;
}

QString QuickEmbeddedShellView::appId() const
{
  return m_appId;
}

void QuickEmbeddedShellView::setAppId(const QString &appId)
{
  if (m_completed) {
    qCWarning(quickShell) << Q_FUNC_INFO << "AppId cannot be changed after the component has been completed!";
    return;
  }

  if (m_appId != appId) {
    m_appId = appId;
    emit appIdChanged(appId);
  }
}

QString QuickEmbeddedShellView::appLabel() const
{
  return m_appLabel;
}

void QuickEmbeddedShellView::setAppLabel(const QString &appLabel)
{
  if (m_appLabel != appLabel) {
    m_appLabel = appLabel;
    emit appLabelChanged(appLabel);
  }
}

QString QuickEmbeddedShellView::appIcon() const
{
  return m_appIcon;
}

void QuickEmbeddedShellView::setAppIcon(const QString &appIcon)
{
  if (m_appIcon != appIcon) {
    m_appIcon = appIcon;
    emit appIconChanged(appIcon);
  }
}

QString QuickEmbeddedShellView::label() const
{
  return m_label;
}

void QuickEmbeddedShellView::setLabel(const QString &label)
{
  if (m_label != label) {
    m_label = label;
    emit labelChanged(label);
  }
}

QString QuickEmbeddedShellView::icon() const
{
  return m_icon;
}

void QuickEmbeddedShellView::setIcon(const QString &icon)
{
  if (m_icon != icon) {
    m_icon = icon;
    emit iconChanged(icon);
  }
}

quint32 QuickEmbeddedShellView::sortIndex() const
{
  return m_sortIndex;
}

void QuickEmbeddedShellView::setSortIndex(quint32 sortIndex)
{
  if (m_sortIndex != sortIndex) {
    m_sortIndex = sortIndex;
    emit sortIndexChanged(sortIndex);
  }
}

QVariant QuickEmbeddedShellView::customData() const
{
  return m_customData;
}

void QuickEmbeddedShellView::setCustomData(const QVariant &customData)
{
  if (m_customData != customData) {
    m_customData = customData;
    emit customDataChanged(customData);
  }
}
