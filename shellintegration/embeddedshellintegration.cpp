#include "embeddedshellintegration.h"
#include "QtWaylandClient/private/qwaylandwindow_p.h"
#include "embeddedshell.h"
#include "embeddedshellsurface.h"

bool EmbeddedShellIntegration::isActive() const { return m_shell->isActive(); }

EmbeddedShellIntegration::EmbeddedShellIntegration()
    : m_shell(new EmbeddedShell()) {
  qDebug() << __PRETTY_FUNCTION__;
}

QWaylandShellSurface *
EmbeddedShellIntegration::createShellSurface(QWaylandWindow *window) {

  EmbeddedShellSurface::Anchor anchor = EmbeddedShellSurface::Anchor::Undefined;

  auto prop = window->window()->property("anchor");
  if (prop.isValid())
    anchor = prop.value<EmbeddedShellSurface::Anchor>();

  uint32_t margin = 0;

  prop = window->window()->property("margin");
  if (prop.isValid())
    margin = prop.toInt();

  qDebug() << __PRETTY_FUNCTION__ << "PROPERTIES" << window->properties()
           << "margin" << margin << "anchor" << anchor;

  auto ess = m_shell->createSurface(window, anchor, margin);
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
    qDebug() << "NOT FOUND";
    return nullptr;
  }
  return QWaylandShellIntegration::nativeResourceForWindow(resource, window);
}
