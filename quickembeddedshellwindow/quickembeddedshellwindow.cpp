#include "quickembeddedshellwindow.h"
#include "embeddedshellsurface.h"
#include "quickembeddedshell.h"
#include <QGuiApplication>
#include <qpa/qplatformnativeinterface.h>

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

void QuickEmbeddedShellWindow::createView() {
  auto view = m_surface->createView();
  qDebug() << __PRETTY_FUNCTION__ << view;
}

void QuickEmbeddedShell::registerTypes(const char *uri) {
  qmlRegisterType<QuickEmbeddedShellWindow>(uri, 1, 0, "Window");
  qmlRegisterType<EmbeddedPlatform>(uri, 1, 0, "Platform");
}

void QuickEmbeddedShellWindow::classBegin() { qDebug() << __PRETTY_FUNCTION__; }

void QuickEmbeddedShellWindow::componentComplete() {
  auto pni = QGuiApplication::platformNativeInterface();
  m_surface = static_cast<EmbeddedShellSurface *>(
      pni->nativeResourceForWindow("embedded-shell-surface", this));

  if (m_surface != nullptr) {
    m_surface->sendAnchor(m_anchor);
    connect(m_surface, &EmbeddedShellSurface::viewCreated, this,
            &QuickEmbeddedShellWindow::viewCreated);
    connect(m_surface, &EmbeddedShellSurface::viewSelected, this,
            &QuickEmbeddedShellWindow::viewSelected);
    // m_surface = EmbeddedPlatform::getSurfaceForWindow(this);
  }
  qDebug() << __PRETTY_FUNCTION__ << m_surface;
}
