#include "quickembeddedshellview.h"
#include "embeddedshellsurface.h"

QuickEmbeddedShellView::QuickEmbeddedShellView(QQuickItem *parent)
    : QQuickItem(parent)
    , m_surface(nullptr)
    , m_selected(false)
    , m_topLevel(false)
    , m_view(nullptr)
    , m_parentView(nullptr)
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
  if (m_surface == surface) {
    return;
  }

  if (!m_completed) {
    m_surface = surface;
    emit surfaceChanged();
  }
  else
  {
    qWarning() << "surface cannot be changed from" << m_surface << "to" << surface << ":" << m_completed;
  }
}

bool QuickEmbeddedShellView::selected() const
{
  return m_selected;
}

bool QuickEmbeddedShellView::topLevel() const
{
  return m_topLevel;
}


void QuickEmbeddedShellView::updateSelected(bool selected)
{
  if (m_selected != selected) {
    m_selected = selected;
    emit selectedChanged();
  }
}

void QuickEmbeddedShellView::updateTopLevel(bool topLevel)
{
  if (m_topLevel != topLevel) {
    m_topLevel = topLevel;
    emit topLevelChanged();
  }
}

EmbeddedShellSurfaceView *QuickEmbeddedShellView::view() const
{
  return m_view;
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

QString QuickEmbeddedShellView::persistentId() const
{
  return m_persistentId;
}

void QuickEmbeddedShellView::setPersistentId(const QString &persistentId)
{
  if (m_persistentId == persistentId) {
    return;
  }

  if (!m_completed)
  {
    m_persistentId = persistentId;
    emit persistentIdChanged();
  }
  else
  {
    qWarning() << "persistentId cannot be changed from" << m_persistentId << "to" << persistentId << ":" << m_completed;
  }
}

QVariantMap QuickEmbeddedShellView::customData() const
{
  return m_customData;
}

void QuickEmbeddedShellView::setCustomData(const QVariantMap &customData)
{
  if (m_customData != customData) {
    m_customData = customData;
    emit customDataChanged();
  }
}

QuickEmbeddedShellView *QuickEmbeddedShellView::parentView() const
{
  return m_parentView;
}

void QuickEmbeddedShellView::setParentView(QuickEmbeddedShellView *parentView)
{
  if (m_parentView == parentView) {
    return;
  }

  if (!m_completed)
  {
    m_parentView = parentView;
    emit parentViewChanged();
  }
  else
  {
    qWarning() << "parentView cannot be changed from" << m_parentView << "to" << parentView << ":" << m_completed;
  }
}

void QuickEmbeddedShellView::createView()
{
  EmbeddedShellSurfaceView *parentView = nullptr;

  if (m_parentView)
  {
    parentView = m_parentView->view();
    if (!parentView)
    {
      connect(m_parentView, &QuickEmbeddedShellView::viewChanged, this, &QuickEmbeddedShellView::createView, Qt::SingleShotConnection);
      return;
    }
  }

  m_view = m_surface->createView(m_label,
                                 m_icon,
                                 m_sortIndex,
                                 m_persistentId,
                                 m_customData,
                                 parentView);
  Q_ASSERT(m_view);

  connect(m_view, &EmbeddedShellSurfaceView::selectedChanged, this, &QuickEmbeddedShellView::updateSelected);
  connect(m_view, &EmbeddedShellSurfaceView::topLevelChanged, this, &QuickEmbeddedShellView::updateTopLevel);
  connect(this, &QuickEmbeddedShellView::labelChanged, m_view, [this]() { m_view->setLabel(label()); });
  connect(this, &QuickEmbeddedShellView::iconChanged, m_view, [this]() { m_view->setIcon(icon()); });
  connect(this, &QuickEmbeddedShellView::sortIndexChanged, m_view, [this]() { m_view->setSortIndex(sortIndex()); });
  connect(this, &QuickEmbeddedShellView::customDataChanged, m_view, [this]() { m_view->setCustomData(customData()); });
  connect(this, &QuickEmbeddedShellView::select, m_view, &EmbeddedShellSurfaceView::select);
  connect(this, &QObject::destroyed, m_view, &QObject::deleteLater);

  m_completed = true;
  emit viewChanged();
}
