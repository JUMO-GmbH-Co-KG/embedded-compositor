// SPDX-License-Identifier: LGPL-3.0-only

#include "quickembeddedshellwindow.h"
#include "embeddedshellsurface.h"

Q_LOGGING_CATEGORY(quickShell, "embeddedshell.quick")

QuickEmbeddedShellWindow::QuickEmbeddedShellWindow(QWindow *parent)
    : QQuickWindow(parent)
    , m_size(QSize{0, 0}) {
  qCDebug(quickShell) << __PRETTY_FUNCTION__;
}

QuickEmbeddedShellWindow::~QuickEmbeddedShellWindow() {}

EmbeddedShellTypes::Anchor QuickEmbeddedShellWindow::anchor() const {
  qCDebug(quickShell) << __PRETTY_FUNCTION__ << m_anchor;
  return m_anchor;
}

void QuickEmbeddedShellWindow::setAnchor(EmbeddedShellTypes::Anchor newAnchor) {
  qCDebug(quickShell) << __PRETTY_FUNCTION__ << m_anchor << "->" << newAnchor;
  if (m_anchor == newAnchor)
    return;
  m_anchor = newAnchor;
  emit anchorChanged(newAnchor);
}

EmbeddedShellSurfaceView *QuickEmbeddedShellWindow::createView(const QString &appId,
                                                               const QString &appLabel,
                                                               const QString &label,
                                                               unsigned int sort_index) {
  auto view = m_surface->createView(appId, appLabel, label, sort_index);
  qCDebug(quickShell) << __PRETTY_FUNCTION__ << appId << appLabel << view << label;
  return view;
}

EmbeddedShellSurfaceView* QuickEmbeddedShellWindow::createView(const QString& appId, const QString& appLabel, const QString& appIcon, const QString& label, const QString& icon, uint32_t sort_index){
    auto view = m_surface->createView(appId, appLabel, appIcon, label, icon, sort_index);
    qCDebug(quickShell) << __PRETTY_FUNCTION__ << appId << appLabel << appIcon << label << icon << view << sort_index;
    return view;
}

void QuickEmbeddedShellWindow::classBegin() {
  qCDebug(quickShell) << __PRETTY_FUNCTION__;
}

void QuickEmbeddedShellWindow::componentComplete() {
  m_surface = EmbeddedPlatform::shellSurfaceForWindow(this);

  if (m_surface != nullptr) {
    m_surface->sendAnchor(m_anchor);
    m_surface->sendMargin(m_margin);
    if (m_size.isValid()) {
      m_surface->sendSize(m_size);
    }
  }
  m_componentComplete = true;
  qCDebug(quickShell) << __PRETTY_FUNCTION__ << m_surface;
}

int QuickEmbeddedShellWindow::implicitWidth() const {
  qCDebug(quickShell) << __PRETTY_FUNCTION__ << m_size.width();
  return m_size.width();
}

void QuickEmbeddedShellWindow::setImplicitWidth(int implicitWidth) {
  qCDebug(quickShell) << __PRETTY_FUNCTION__ << implicitWidth;
  const QSize newSize{implicitWidth, m_size.height()};
  if (m_size == newSize) {
    return;
  }
  m_size = newSize;
  Q_EMIT implicitWidthChanged(implicitWidth);
  if (m_componentComplete && m_surface) {
    m_surface->sendSize(m_size);
  }
}

int QuickEmbeddedShellWindow::implicitHeight() const {
  qCDebug(quickShell) << __PRETTY_FUNCTION__ << m_size.height();
  return m_size.height();
}

void QuickEmbeddedShellWindow::setImplicitHeight(int implicitHeight) {
  qCDebug(quickShell) << __PRETTY_FUNCTION__ << implicitHeight;
  const QSize newSize{m_size.width(), implicitHeight};
  if (m_size == newSize) {
    return;
  }
  m_size = newSize;
  Q_EMIT implicitHeightChanged(implicitHeight);
  if (m_componentComplete && m_surface) {
    m_surface->sendSize(m_size);
  }
}

int QuickEmbeddedShellWindow::margin() const {
  qCDebug(quickShell) << __PRETTY_FUNCTION__ << m_margin;
  return m_margin;
}

void QuickEmbeddedShellWindow::setMargin(int newMargin) {
  qCDebug(quickShell) << __PRETTY_FUNCTION__ << newMargin;
  if (m_margin == newMargin)
    return;
  m_margin = newMargin;
  emit marginChanged(newMargin);
}

unsigned int QuickEmbeddedShellWindow::sortIndex() const { return m_sortIndex; }

void QuickEmbeddedShellWindow::setSortIndex(unsigned int sortIndex) {
  qCDebug(quickShell) << __PRETTY_FUNCTION__ << sortIndex;
  if (m_sortIndex == sortIndex)
    return;
  m_sortIndex = sortIndex;
  emit sortIndexChanged(sortIndex);
}
