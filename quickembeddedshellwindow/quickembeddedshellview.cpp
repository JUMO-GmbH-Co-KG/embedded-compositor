#include "quickembeddedshellview.h"
#include "embeddedshellsurface.h"

QuickEmbeddedShellView::QuickEmbeddedShellView(QQuickItem *parent)
    : QQuickItem(parent)
    , m_surface(nullptr)
    , m_isCurrentView(false)
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
    Q_EMIT surfaceChanged(surface);
  }
}

bool QuickEmbeddedShellView::isCurrentView() const
{
  return m_isCurrentView;
}

void QuickEmbeddedShellView::setIsCurrentView(bool isCurrentView)
{
  if (m_isCurrentView != isCurrentView) {
    m_isCurrentView = isCurrentView;
    Q_EMIT isCurrentViewChanged(isCurrentView);
  }
}

void QuickEmbeddedShellView::createView()
{
  auto view = m_surface->createView(m_appId, m_appLabel, m_appIcon, m_label, m_icon, m_sortIndex);
  Q_ASSERT(view);

  connect(view, &EmbeddedShellSurfaceView::selected, this, [view, this]() {
    setIsCurrentView(true);
  });
  connect(view, &EmbeddedShellSurfaceView::deselected, this, [view, this]() {
    setIsCurrentView(false);
  });
  connect(this, &QuickEmbeddedShellView::appLabelChanged, view, &EmbeddedShellSurfaceView::setAppLabel);
  connect(this, &QuickEmbeddedShellView::appIconChanged, view, &EmbeddedShellSurfaceView::setAppIcon);
  connect(this, &QuickEmbeddedShellView::labelChanged, view, &EmbeddedShellSurfaceView::setLabel);
  connect(this, &QuickEmbeddedShellView::iconChanged, view, &EmbeddedShellSurfaceView::setIcon);
  connect(this, &QuickEmbeddedShellView::sortIndexChanged, view, &EmbeddedShellSurfaceView::setSortIndex);
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
    qCWarning(quickShell) << __PRETTY_FUNCTION__ << "AppId cannot be changed after the component has been completed!";
    return;
  }

  if (m_appId != appId) {
    m_appId = appId;
    Q_EMIT appIdChanged(appId);
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
    Q_EMIT appLabelChanged(appLabel);
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
    Q_EMIT appIconChanged(appIcon);
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
    Q_EMIT labelChanged(label);
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
    Q_EMIT iconChanged(icon);
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
    Q_EMIT sortIndexChanged(sortIndex);
  }
}
