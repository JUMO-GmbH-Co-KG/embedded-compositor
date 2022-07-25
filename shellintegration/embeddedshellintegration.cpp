#include "embeddedshellintegration.h"
#include "QtWaylandClient/private/qwaylandwindow_p.h"
#include "embeddedshell.h"
#include "embeddedshellsurface.h"

bool EmbeddedShellIntegration::isActive() const { return m_shell->isActive(); }

EmbeddedShellIntegration::EmbeddedShellIntegration()
    : m_shell(new EmbeddedShell()) {}

QWaylandShellSurface *
EmbeddedShellIntegration::createShellSurface(QWaylandWindow *window) {
  EmbeddedShellSurface::Anchor anchor = EmbeddedShellSurface::Anchor::Undefined;
  auto ess = m_shell->createSurface(window, anchor);
  if (ess == nullptr)
    return nullptr;
  m_windows.insert(window, ess);
  return ess->shellSurface();
}

bool EmbeddedShellIntegration::initialize(QWaylandDisplay *display) {
  QWaylandShellIntegration::initialize(display);
  qDebug() << __PRETTY_FUNCTION__ << "active" << m_shell->isActive();
  return m_shell->isActive();
}

void *
EmbeddedShellIntegration::nativeResourceForWindow(const QByteArray &resource,
                                                  QWindow *window) {
  qDebug() << __PRETTY_FUNCTION__ << resource << window << window->handle();

  if (resource == "embedded-shell-surface") {
    auto qww = static_cast<QWaylandWindow *>(window->handle());
    auto found = m_windows.find(qww);
    if (found != m_windows.end()) {
      qDebug() << "found";
      return found.value();
    }
    return nullptr;
  }
  return QWaylandShellIntegration::nativeResourceForWindow(resource, window);
}
