#include "embeddedshellintegration.h"
#include "QtWaylandClient/private/qwaylandwindow_p.h"
#include "embeddedshellsurface.h"

EmbeddedShellIntegration::EmbeddedShellIntegration()
    : QWaylandClientExtension(1)

{
  qDebug() << __PRETTY_FUNCTION__ << "active?" << isActive();
}

QWaylandShellSurface *
EmbeddedShellIntegration::createShellSurface(QWaylandWindow *window) {
  qDebug() << __PRETTY_FUNCTION__ << "active? " << isActive();
  if (!isActive())
    return nullptr;
  EmbeddedShellSurface::Anchor anchor = EmbeddedShellSurface::Anchor::Undefined;
  auto *surface =
      surface_create(window->wlSurface(), (embedded_shell_anchor_border)anchor);
  auto ess = new EmbeddedShellSurface(surface, window, anchor);
  m_windows.insert(window, ess);
  return ess->shellSurface();
}

bool EmbeddedShellIntegration::initialize(QWaylandDisplay *display) {
  QWaylandShellIntegration::initialize(display);
  qDebug() << __PRETTY_FUNCTION__ << "active" << isActive();
  return isActive();
}

const wl_interface *EmbeddedShellIntegration::extensionInterface() const {
  return QtWayland::embedded_shell::interface();
}

void EmbeddedShellIntegration::bind(wl_registry *registry, int id, int ver) {
  auto *instance = static_cast<QtWayland::embedded_shell *>(this);
  // Make sure lowest version is used of the supplied version from the
  // developer and the version specified in the protocol and also the
  // compositor version.
  if (this->version() > QtWayland::embedded_shell::interface()->version) {
    qWarning("Supplied protocol version to QWaylandClientExtensionTemplate is "
             "higher than the version of the protocol, using protocol version "
             "instead.");
  }
  int minVersion =
      qMin(ver, qMin(QtWayland::embedded_shell::interface()->version,
                     this->version()));
  setVersion(minVersion);
  instance->init(registry, id, minVersion);
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
