#include "quickembeddedshellwindow.h"
#include "embeddedshellsurface.h"
#include "quickembeddedshell.h"

QuickEmbeddedShellWindow::QuickEmbeddedShellWindow(QWindow *parent)
    : QQuickWindow(parent) {
  qDebug() << __PRETTY_FUNCTION__;
}

QuickEmbeddedShellWindow::~QuickEmbeddedShellWindow() {}

QuickEmbeddedShellWindow::Anchor QuickEmbeddedShellWindow::anchor() const {
  qDebug() << __PRETTY_FUNCTION__ << m_anchor;
  return m_anchor;
}

void QuickEmbeddedShellWindow::setAnchor(Anchor newAnchor) {
  qDebug() << __PRETTY_FUNCTION__ << m_anchor << "->" << newAnchor;
  if (m_anchor == newAnchor)
    return;
  m_anchor = newAnchor;
  emit anchorChanged(newAnchor);
}

EmbeddedShellSurfaceView *QuickEmbeddedShellWindow::createView(QString label) {
  auto view = m_surface->createView(label);
  qDebug() << __PRETTY_FUNCTION__ << view << label;
  return view;
}

void QuickEmbeddedShell::registerTypes(const char *uri) {
  qmlRegisterType<QuickEmbeddedShellWindow>(uri, 1, 0, "Window");
  qmlRegisterType<EmbeddedPlatform>(uri, 1, 0, "Platform");
  qmlRegisterUncreatableType<EmbeddedShellSurfaceView>(
      uri, 1, 0, "SurfaceView", "created by wayland request only");
}

void QuickEmbeddedShellWindow::classBegin() { qDebug() << __PRETTY_FUNCTION__; }

void QuickEmbeddedShellWindow::componentComplete() {
  m_surface = EmbeddedPlatform::shellSurfaceForWindow(this);

  if (m_surface != nullptr) {
    m_surface->sendAnchor(m_anchor);
  }
  qDebug() << __PRETTY_FUNCTION__ << m_surface;
}
