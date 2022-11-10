#include "quickembeddedshellwindow.h"
#include "embeddedshellsurface.h"
#include "quickembeddedshell.h"

Q_LOGGING_CATEGORY(quickShell, "embeddedshell.quick")

QuickEmbeddedShellWindow::QuickEmbeddedShellWindow(QWindow *parent)
    : QQuickWindow(parent) {
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

EmbeddedShellSurfaceView *QuickEmbeddedShellWindow::createView(QString label,
                                                               int sort_index) {
  auto view = m_surface->createView(label, sort_index);
  qCDebug(quickShell) << __PRETTY_FUNCTION__ << view << label;
  return view;
}

void QuickEmbeddedShell::registerTypes(const char *uri) {
  qmlRegisterType<QuickEmbeddedShellWindow>(uri, 1, 0, "Window");
  qmlRegisterType<EmbeddedPlatform>(uri, 1, 0, "Platform");
  qmlRegisterUncreatableType<EmbeddedShellSurfaceView>(
      uri, 1, 0, "SurfaceView", "created by wayland request only");
  qmlRegisterUncreatableMetaObject(EmbeddedShellTypes::staticMetaObject, uri, 1,
                                   0, "EmbeddedShellTypes",
                                   "uncreatable enums namespace");
}

void QuickEmbeddedShellWindow::classBegin() {
  qCDebug(quickShell) << __PRETTY_FUNCTION__;
}

void QuickEmbeddedShellWindow::componentComplete() {
  m_surface = EmbeddedPlatform::shellSurfaceForWindow(this);

  if (m_surface != nullptr) {
    m_surface->sendAnchor(m_anchor);
    m_surface->sendMargin(m_margin);
  }
  qCDebug(quickShell) << __PRETTY_FUNCTION__ << m_surface;
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

int QuickEmbeddedShellWindow::sortIndex() const { return m_sortIndex; }

void QuickEmbeddedShellWindow::setSortIndex(int sortIndex) {
  qCDebug(quickShell) << __PRETTY_FUNCTION__ << sortIndex;
  if (m_sortIndex == sortIndex)
    return;
  m_sortIndex = sortIndex;
  emit sortIndexChanged(sortIndex);
}
