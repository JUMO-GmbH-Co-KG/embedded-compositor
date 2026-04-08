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
    qCCritical(quickShell) << Q_FUNC_INFO << "called without a window set; aborting component completion.";
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
      m_surface->setSize(m_size);
    }
    m_surface->setAnchor(m_anchor);
    m_surface->setMargin(m_margin);

    connect(m_surface, &EmbeddedShellSurface::visibleChanged, this, &QuickEmbeddedShellSurface::visibleChanged);

    if (m_surface->visible())
    {
      emit visibleChanged();
    }

    m_componentComplete = true;
    emit completedChanged();
    qCDebug(quickShell) << Q_FUNC_INFO << m_surface << m_anchor << m_margin << m_size;
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

  qCDebug(quickShell) << Q_FUNC_INFO << m_anchor << "->" << newAnchor;

  m_anchor = newAnchor;
  emit anchorChanged();

  if (m_componentComplete && m_surface) {
    m_surface->setAnchor(m_anchor);
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
    emit windowChanged();
  } else {
    qCWarning(quickShell) << "Invalid window assignment from" << m_window << "to" << window << "!";
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

  qCDebug(quickShell) << Q_FUNC_INFO << implicitWidth;

  m_size = newSize;
  emit implicitWidthChanged();

  if (m_componentComplete && m_surface) {
    m_surface->setSize(m_size);
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

  qCDebug(quickShell) << Q_FUNC_INFO << implicitHeight;

  m_size = newSize;
  emit implicitHeightChanged();

  if (m_componentComplete && m_surface) {
    m_surface->setSize(m_size);
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

  qCDebug(quickShell) << Q_FUNC_INFO << newMargin;

  m_margin = newMargin;
  emit marginChanged();

  if (m_componentComplete && m_surface) {
    m_surface->setMargin(m_margin);
  }
}

bool QuickEmbeddedShellSurface::completed() const
{
  return m_componentComplete;
}

bool QuickEmbeddedShellSurface::visible() const
{
  return m_surface && m_surface->visible();
}

EmbeddedShellSurfaceView* QuickEmbeddedShellSurface::createView(const QString& label,
                                                                const QString& icon,
                                                                uint32_t sortIndex,
                                                                const QVariant &customData,
                                                                EmbeddedShellSurfaceView *parentView)
{
  if (m_surface) {
    auto view = m_surface->createView(label, icon, sortIndex, customData, parentView);
    qCDebug(quickShell) << Q_FUNC_INFO << label << icon << view << sortIndex << customData << parentView;
    return view;
  } else {
    qCDebug(quickShell) << Q_FUNC_INFO << "Surface has not been created yet!";
    return nullptr;
  }
}
