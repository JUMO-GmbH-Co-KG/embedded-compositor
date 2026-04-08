// SPDX-License-Identifier: LGPL-3.0-only

#include "embeddedshell.h"
#include "QtWaylandClient/private/qwaylandwindow_p.h"
#include "qwayland-embedded-shell.h"

Q_LOGGING_CATEGORY(shellExt, "embeddedshell.client")

EmbeddedShell::EmbeddedShell(QtWayland::embedded_shell *embeddedShell)
  : m_embeddedShell(embeddedShell)
{
  qCDebug(shellExt) << Q_FUNC_INFO;
}

EmbeddedShellSurface *EmbeddedShell::createSurface(QtWaylandClient::QWaylandWindow *window,
                                                   EmbeddedShellTypes::Anchor anchor,
                                                   uint32_t margin) {
  qCDebug(shellExt) << Q_FUNC_INFO << anchor << margin;
  auto surface = m_embeddedShell->surface_create(
      window->wlSurface(),
      static_cast<embedded_shell_anchor_border>(anchor),
      margin);

  const QSize size{0, 0};
  auto ess = new EmbeddedShellSurface(surface,
                                      window,
                                      size,
                                      anchor,
                                      margin);
  return ess;
}
