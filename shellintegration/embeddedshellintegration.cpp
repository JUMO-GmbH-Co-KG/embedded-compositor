#include "embeddedshellintegration.h"
#include "QtWaylandClient/private/qwaylandwindow_p.h"
#include "embeddedshell.h"
#include "embeddedshellsurface.h"

bool EmbeddedShellIntegration::isActive() const { return m_shell->isActive(); }

EmbeddedShellIntegration::EmbeddedShellIntegration()
    : m_shell(new EmbeddedShell()) {}

QtWaylandClient::QWaylandShellSurface *
EmbeddedShellIntegration::createShellSurface(
    QtWaylandClient::QWaylandWindow *window) {

  EmbeddedShellTypes::Anchor anchor = EmbeddedShellTypes::Anchor::Undefined;

  auto prop = window->window()->property("anchor");
  if (prop.isValid())
    anchor = prop.value<EmbeddedShellTypes::Anchor>();

  uint32_t margin = 0;

  prop = window->window()->property("margin");
  if (prop.isValid())
    margin = prop.toUInt();

  int32_t sort_index = 0;
  prop = window->window()->property("sortIndex");
  if (prop.isValid())
    sort_index = prop.toInt();

  auto ess = m_shell->createSurface(window, anchor, margin, sort_index);

  if (ess == nullptr) {
    return nullptr;
  }

  m_windows.insert(window, ess);
  emit EmbeddedPlatform::instance()->shellSurfaceCreated(ess, window->window());
  return ess->shellSurface();
}

bool EmbeddedShellIntegration::initialize(
    QtWaylandClient::QWaylandDisplay *display) {
  QWaylandShellIntegration::initialize(display);
  return m_shell->isActive();
}

void *
EmbeddedShellIntegration::nativeResourceForWindow(const QByteArray &resource,
                                                  QWindow *window) {
  if (resource == "embedded-shell-surface") {
    auto qww = static_cast<QtWaylandClient::QWaylandWindow *>(window->handle());
    auto found = m_windows.find(qww);
    if (found != m_windows.end()) {
      return found.value();
    }
    qDebug() << __PRETTY_FUNCTION__ << " ... not found";
    return nullptr;
  }
  return QWaylandShellIntegration::nativeResourceForWindow(resource, window);
}
