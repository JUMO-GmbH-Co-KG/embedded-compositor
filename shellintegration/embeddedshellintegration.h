// SPDX-License-Identifier: LGPL-3.0-only

#ifndef EMBEDDEDSHELLINTEGRATION_H
#define EMBEDDEDSHELLINTEGRATION_H

#include <QScopedPointer>
#include <QtWaylandClient/private/qwaylandshellintegration_p.h>

#include "qwayland-embedded-shell.h"

class EmbeddedShellSurface;
class EmbeddedShell;

class
#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)
Q_WAYLAND_CLIENT_EXPORT
#else
Q_WAYLANDCLIENT_EXPORT
#endif
EmbeddedShellIntegration
  : public QtWaylandClient::QWaylandShellIntegrationTemplate<EmbeddedShellIntegration>
  , public QtWayland::embedded_shell
{
public:
  EmbeddedShellIntegration();
  QtWaylandClient::QWaylandShellSurface *
  createShellSurface(QtWaylandClient::QWaylandWindow *window) override;
  // QWaylandShellIntegration interface
  void *nativeResourceForWindow(const QByteArray &resource,
                                QWindow *window) override;

private:
  QScopedPointer<EmbeddedShell> m_shell;
  QMap<QtWaylandClient::QWaylandWindow *, EmbeddedShellSurface *> m_windows;
};

#endif // EMBEDDEDSHELLINTEGRATION_H
