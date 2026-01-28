#include "quickembeddedshellview.h"
#include "embeddedshellsurface.h"

QuickEmbeddedShellView::QuickEmbeddedShellView(QQuickItem *parent)
    : QQuickItem(parent)
    , m_Window(nullptr)
    , m_IsCurrentView(false)
    , m_SortIndex(0)
    , m_Completed(false)
{
}

void QuickEmbeddedShellView::componentComplete()
{
  Q_ASSERT(m_Window);

  if (m_Window->completed()) {
    auto view = m_Window->createView(m_AppId, m_AppLabel, m_AppIcon, m_Label, m_Icon, m_SortIndex);
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

    m_Completed = true;
  } else {
    connect(m_Window, &QuickEmbeddedShellWindow::completedChanged, this, &QuickEmbeddedShellView::componentComplete, Qt::SingleShotConnection);
  }

  QQuickItem::componentComplete();
}

QuickEmbeddedShellWindow *QuickEmbeddedShellView::window() const
{
  return m_Window;
}

void QuickEmbeddedShellView::setWindow(QuickEmbeddedShellWindow *window)
{
  if (!m_Window && window) {
    m_Window = window;
    Q_EMIT quickEmbeddedShellWindowChanged(window);
  }
}

bool QuickEmbeddedShellView::isCurrentView() const
{
  return m_IsCurrentView;
}

void QuickEmbeddedShellView::setIsCurrentView(bool isCurrentView)
{
  if (m_IsCurrentView != isCurrentView) {
    m_IsCurrentView = isCurrentView;
    Q_EMIT isCurrentViewChanged(isCurrentView);
  }
}

QString QuickEmbeddedShellView::appId() const
{
  return m_AppId;
}

void QuickEmbeddedShellView::setAppId(const QString &appId)
{
  if (m_Completed) {
    qCWarning(quickShell) << __PRETTY_FUNCTION__ << "AppId cannot be changed after the component has been completed!";
    return;
  }

  if (m_AppId != appId) {
    m_AppId = appId;
    Q_EMIT appIdChanged(appId);
  }
}

QString QuickEmbeddedShellView::appLabel() const
{
  return m_AppLabel;
}

void QuickEmbeddedShellView::setAppLabel(const QString &appLabel)
{
  if (m_AppLabel != appLabel) {
    m_AppLabel = appLabel;
    Q_EMIT appLabelChanged(appLabel);
  }
}

QString QuickEmbeddedShellView::appIcon() const
{
  return m_AppIcon;
}

void QuickEmbeddedShellView::setAppIcon(const QString &appIcon)
{
  if (m_AppIcon != appIcon) {
    m_AppIcon = appIcon;
    Q_EMIT appIconChanged(appIcon);
  }
}

QString QuickEmbeddedShellView::label() const
{
  return m_Label;
}

void QuickEmbeddedShellView::setLabel(const QString &label)
{
  if (m_Label != label) {
    m_Label = label;
    Q_EMIT labelChanged(label);
  }
}

QString QuickEmbeddedShellView::icon() const
{
  return m_Icon;
}

void QuickEmbeddedShellView::setIcon(const QString &icon)
{
  if (m_Icon != icon) {
    m_Icon = icon;
    Q_EMIT iconChanged(icon);
  }
}

quint32 QuickEmbeddedShellView::sortIndex() const
{
  return m_SortIndex;
}

void QuickEmbeddedShellView::setSortIndex(quint32 sortIndex)
{
  if (m_SortIndex != sortIndex) {
    m_SortIndex = sortIndex;
    Q_EMIT sortIndexChanged(sortIndex);
  }
}
