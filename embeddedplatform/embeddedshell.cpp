#include "embeddedshell.h"
#include "QtWaylandClient/private/qwaylandwindow_p.h"
#include "qwayland-embedded-shell.h"

Q_LOGGING_CATEGORY(shellExt, "embeddedshell.client")

EmbeddedShell::EmbeddedShell()
    : QWaylandClientExtension(/*version=*/1),
      instance(new QtWayland::embedded_shell()) {
  qCDebug(shellExt) << __PRETTY_FUNCTION__ << isActive();
}

EmbeddedShellSurface *
EmbeddedShell::createSurface(QtWaylandClient::QWaylandWindow *window,
                             EmbeddedShellTypes::Anchor anchor, uint32_t margin,
                             int sort_index) {
  qCDebug(shellExt) << __PRETTY_FUNCTION__ << isActive() << anchor << margin;
  if (!isActive())
    return nullptr;
  auto surface = instance->surface_create(
      window->wlSurface(), static_cast<embedded_shell_anchor_border>(anchor),
      margin, sort_index);
  auto ess =
      new EmbeddedShellSurface(surface, window, anchor, margin, sort_index);
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
    qCWarning(shellExt)
        << "Supplied protocol version to QWaylandClientExtensionTemplate is "
           "higher than the version of the protocol, using protocol version "
           "instead.";
  }
  int minVersion =
      qMin(ver, qMin(QtWayland::embedded_shell::interface()->version,
                     this->version()));
  setVersion(minVersion);
  instance->init(registry, id, minVersion);
}
