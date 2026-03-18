// SPDX-License-Identifier: LGPL-3.0-only

#include "quickembeddedshellsurface.h"
#include "embeddedshellsurface.h"

#include <QQmlApplicationEngine>

#include <memory>

Q_LOGGING_CATEGORY(quickShell, "embeddedshell.quick")

QuickEmbeddedShellSurface::QuickEmbeddedShellSurface(QObject *parent)
    : QObject(parent)
    , m_window(nullptr)
    , m_size(QSize{0, 0})
    , m_anchor(EmbeddedShellTypes::Anchor::Undefined)
    , m_surface(nullptr)
    , m_margin(-1)
    , m_sortIndex(0)
    , m_componentComplete(false)
    , m_visible(false)
{
}

void QuickEmbeddedShellSurface::classBegin()
{
}

void QuickEmbeddedShellSurface::componentComplete()
{
  if (!m_window) {
    qCCritical(quickShell) << __PRETTY_FUNCTION__ << "called without a window set; aborting component completion.";
    return;
  }

  auto surface = EmbeddedPlatform::shellSurfaceForWindow(m_window);

  if (!surface) {
    auto engine = dynamic_cast<QQmlApplicationEngine *>(qmlEngine(m_window));
    // If the surface is null, the component creation of the window has not yet been completed.

    if (engine) {
      // When the QML engine is of type QQmlApplicationEngine, we can deduce component completion via
      // QQmlApplicationEngine::objectCreated.
      auto connection = std::make_shared<QMetaObject::Connection>();
      *connection = connect(engine, &QQmlApplicationEngine::objectCreated, this, [this, connection](QObject *object) {
        if (object == m_window) {
          disconnect(*connection);
          componentComplete();
        }
      });
    } else {
      // If it isn't of type QQmlApplicationEngine, we use a queued connection as a fall back fall back since
      // component creation usually is resolved one event loop cycle later.
      QMetaObject::invokeMethod(this, &QuickEmbeddedShellSurface::componentComplete, Qt::QueuedConnection);
    }
  }
  else
  {
    m_surface = surface;
    if (m_size.isValid()) {
      m_surface->sendSize(m_size);
    }
    m_surface->sendAnchor(m_anchor);
    m_surface->sendMargin(m_margin);
    m_surface->sendSortIndex(m_sortIndex);
    m_surface->sendAppId(m_appId);
    m_surface->sendAppLabel(m_appLabel);
    m_surface->sendAppIcon(m_appIcon);

    connect(m_surface, &EmbeddedShellSurface::visibleChanged, this, &QuickEmbeddedShellSurface::visibleChanged);

    if (m_surface->getVisible())
    {
      emit visibleChanged(true);
    }

    m_componentComplete = true;
    Q_EMIT completedChanged(m_componentComplete);
    qCDebug(quickShell) << __PRETTY_FUNCTION__ << m_surface << m_anchor << m_margin << m_size << m_sortIndex;
  }
}

QWindow *QuickEmbeddedShellSurface::window() const
{
  return m_window;
}

void QuickEmbeddedShellSurface::setWindow(QWindow *window)
{
  if (!m_window && window) {
    m_window = window;
    Q_EMIT windowChanged(window);
  } else {
    qCWarning(quickShell) << "Invalid window assignment from" << m_window << "to" << window << "!";
  }
}

EmbeddedShellTypes::Anchor QuickEmbeddedShellSurface::anchor() const
{
  return m_anchor;
}

void QuickEmbeddedShellSurface::setAnchor(EmbeddedShellTypes::Anchor newAnchor)
{
  if (m_anchor == newAnchor) {
    return;
  }

  qCDebug(quickShell) << __PRETTY_FUNCTION__ << m_anchor << "->" << newAnchor;

  m_anchor = newAnchor;
  Q_EMIT anchorChanged(newAnchor);

  if (m_componentComplete && m_surface) {
    m_surface->sendAnchor(m_anchor);
  }
}



int QuickEmbeddedShellSurface::implicitWidth() const
{
  return m_size.width();
}

void QuickEmbeddedShellSurface::setImplicitWidth(int implicitWidth)
{
  const QSize newSize{implicitWidth, m_size.height()};
  if (m_size == newSize) {
    return;
  }

  qCDebug(quickShell) << __PRETTY_FUNCTION__ << implicitWidth;

  m_size = newSize;
  Q_EMIT implicitWidthChanged(implicitWidth);

  if (m_componentComplete && m_surface) {
    m_surface->sendSize(m_size);
  }
}

int QuickEmbeddedShellSurface::implicitHeight() const
{
  return m_size.height();
}

void QuickEmbeddedShellSurface::setImplicitHeight(int implicitHeight)
{
  const QSize newSize{m_size.width(), implicitHeight};
  if (m_size == newSize) {
    return;
  }

  qCDebug(quickShell) << __PRETTY_FUNCTION__ << implicitHeight;

  m_size = newSize;
  Q_EMIT implicitHeightChanged(implicitHeight);

  if (m_componentComplete && m_surface) {
    m_surface->sendSize(m_size);
  }
}

int QuickEmbeddedShellSurface::margin() const
{
  return m_margin;
}

void QuickEmbeddedShellSurface::setMargin(int newMargin)
{
  if (m_margin == newMargin) {
    return;
  }

  qCDebug(quickShell) << __PRETTY_FUNCTION__ << newMargin;

  m_margin = newMargin;
  Q_EMIT marginChanged(newMargin);

  if (m_componentComplete && m_surface) {
    m_surface->sendMargin(m_margin);
  }
}

unsigned int QuickEmbeddedShellSurface::sortIndex() const
{
  return m_sortIndex;
}

void QuickEmbeddedShellSurface::setSortIndex(unsigned int sortIndex)
{
  if (m_sortIndex == sortIndex) {
    return;
  }

  qCDebug(quickShell) << __PRETTY_FUNCTION__ << sortIndex;

  m_sortIndex = sortIndex;
  Q_EMIT sortIndexChanged(sortIndex);

  if (m_componentComplete && m_surface) {
    m_surface->sendSortIndex(m_sortIndex);
  }
}

QString QuickEmbeddedShellSurface::appId() const
{
  return m_appId;
}

void QuickEmbeddedShellSurface::setAppId(const QString &appId)
{
  if (m_componentComplete) {
    qCWarning(quickShell) << __PRETTY_FUNCTION__ << "AppId cannot be changed after the component has been completed!";
    return;
  }

  if (m_appId == appId) {
    return;
  }

  qCDebug(quickShell) << __PRETTY_FUNCTION__ << appId;

  m_appId = appId;
  Q_EMIT appIdChanged(appId);
}

QString QuickEmbeddedShellSurface::appLabel() const
{
  return m_appLabel;
}

void QuickEmbeddedShellSurface::setAppLabel(const QString &appLabel)
{
  if (m_appLabel == appLabel) {
    return;
  }

  qCDebug(quickShell) << __PRETTY_FUNCTION__ << appLabel;

  m_appLabel = appLabel;
  Q_EMIT appLabelChanged(appLabel);

  if (m_componentComplete && m_surface) {
    m_surface->sendAppLabel(m_appLabel);
  }
}

QString QuickEmbeddedShellSurface::appIcon() const
{
  return m_appIcon;
}

void QuickEmbeddedShellSurface::setAppIcon(const QString &appIcon)
{
  if (m_appIcon == appIcon) {
    return;
  }

  qCDebug(quickShell) << __PRETTY_FUNCTION__ << appIcon;

  m_appIcon = appIcon;
  Q_EMIT appIconChanged(appIcon);

  if (m_componentComplete && m_surface) {
    m_surface->sendAppIcon(m_appIcon);
  }
}

bool QuickEmbeddedShellSurface::completed() const
{
  return m_componentComplete;
}

bool QuickEmbeddedShellSurface::visible() const
{
  return m_surface && m_surface->getVisible();
}

EmbeddedShellSurfaceView *QuickEmbeddedShellSurface::createView(const QString &appId,
                                                                const QString &appLabel,
                                                                const QString &label,
                                                                unsigned int sort_index)
{
  if (m_surface) {
    auto view = m_surface->createView(appId, appLabel, label, sort_index);
    qCDebug(quickShell) << __PRETTY_FUNCTION__ << appId << appLabel << view << label;
    return view;
  } else {
    qCDebug(quickShell) << __PRETTY_FUNCTION__ << "Surface has not been created yet!";
    return nullptr;
  }
}

EmbeddedShellSurfaceView* QuickEmbeddedShellSurface::createView(const QString& appId,
                                                                const QString& appLabel,
                                                                const QString& appIcon,
                                                                const QString& label,
                                                                const QString& icon,
                                                                uint32_t sort_index)
{
  if (m_surface) {
    auto view = m_surface->createView(appId, appLabel, appIcon, label, icon, sort_index);
    qCDebug(quickShell) << __PRETTY_FUNCTION__ << appId << appLabel << appIcon << label << icon << view << sort_index;
    return view;
  } else {
    qCDebug(quickShell) << __PRETTY_FUNCTION__ << "Surface has not been created yet!";
    return nullptr;
  }
}
