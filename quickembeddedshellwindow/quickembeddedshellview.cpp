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
    emit surfaceChanged();
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
    emit selectedChanged();
  }
}

void QuickEmbeddedShellView::createView()
{
  auto view = m_surface->createView(m_appId, m_appLabel, m_appIcon, m_label, m_icon, m_sortIndex, m_customData);
  Q_ASSERT(view);

  connect(view, &EmbeddedShellSurfaceView::selectedChanged, this, &QuickEmbeddedShellView::setSelected);
  connect(this, &QuickEmbeddedShellView::appLabelChanged, view, [this, view]() { view->setAppLabel(appLabel()); });
  connect(this, &QuickEmbeddedShellView::appIconChanged, view, [this, view]() { view->setAppIcon(appIcon()); });
  connect(this, &QuickEmbeddedShellView::labelChanged, view, [this, view]() { view->setLabel(label()); });
  connect(this, &QuickEmbeddedShellView::iconChanged, view, [this, view]() { view->setIcon(icon()); });
  connect(this, &QuickEmbeddedShellView::sortIndexChanged, view, [this, view]() { view->setSortIndex(sortIndex()); });
  connect(this, &QuickEmbeddedShellView::customDataChanged, view, [this, view]() { view->setCustomData(customData()); });
  connect(this, &QuickEmbeddedShellView::select, view, &EmbeddedShellSurfaceView::select);
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
    emit appIdChanged();
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
    emit appLabelChanged();
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
    emit appIconChanged();
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
    emit labelChanged();
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
    emit iconChanged();
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
    emit sortIndexChanged();
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
    emit customDataChanged();
  }
}
