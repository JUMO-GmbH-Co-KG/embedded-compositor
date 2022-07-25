#include "embeddedshell.h"
#include "QtWaylandClient/private/qwaylandwindow_p.h"
#include "qwayland-embedded-shell.h"

EmbeddedShell::EmbeddedShell()
    : QWaylandClientExtension(1), instance(new QtWayland::embedded_shell()) {}

EmbeddedShellSurface *
EmbeddedShell::createSurface(QtWaylandClient::QWaylandWindow *window,
                             EmbeddedShellSurface::Anchor anchor) {
  if (!isActive())
    return nullptr;
  auto surface = instance->surface_create(window->wlSurface(),
                                          (embedded_shell_anchor_border)anchor);
  auto ess = new EmbeddedShellSurface(surface, window, anchor);
  return ess;
}

const wl_interface *EmbeddedShell::extensionInterface() const {
  return instance->interface();
}

void EmbeddedShell::bind(wl_registry *registry, int id, int ver) {
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
