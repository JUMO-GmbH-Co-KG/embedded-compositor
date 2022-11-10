#ifndef EMBEDDEDSHELLINTEGRATION_H
#define EMBEDDEDSHELLINTEGRATION_H
#include <QScopedPointer>
#include <QtWaylandClient/private/qwaylandclientextension_p.h>
#include <QtWaylandClient/private/qwaylandshellintegration_p.h>

class EmbeddedShellSurface;
class EmbeddedShell;

class Q_WAYLAND_CLIENT_EXPORT EmbeddedShellIntegration
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
