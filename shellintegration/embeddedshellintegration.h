// SPDX-License-Identifier: LGPL-3.0-only

#ifndef EMBEDDEDSHELLINTEGRATION_H
#define EMBEDDEDSHELLINTEGRATION_H

#include <QScopedPointer>
#include <QtWaylandClient/private/qwaylandclientextension_p.h>
#include <QtWaylandClient/private/qwaylandshellintegration_p.h>

class EmbeddedShellSurface;
class EmbeddedShell;

class
#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)
Q_WAYLAND_CLIENT_EXPORT
#else
Q_WAYLANDCLIENT_EXPORT
#endif
        EmbeddedShellIntegration
    : public QtWaylandClient::QWaylandShellIntegration {
  QMap<QtWaylandClient::QWaylandWindow *, EmbeddedShellSurface *> m_windows;
  QScopedPointer<EmbeddedShell> m_shell;

public:
  bool isActive() const;
  EmbeddedShellIntegration();
  QtWaylandClient::QWaylandShellSurface *
  createShellSurface(QtWaylandClient::QWaylandWindow *window) override;
  bool initialize(QtWaylandClient::QWaylandDisplay *display) override;
  // QWaylandShellIntegration interface
  void *nativeResourceForWindow(const QByteArray &resource,
                                QWindow *window) override;
};

#endif // EMBEDDEDSHELLINTEGRATION_H
