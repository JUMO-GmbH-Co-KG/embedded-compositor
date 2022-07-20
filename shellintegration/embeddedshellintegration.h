#ifndef EMBEDDEDSHELLINTEGRATION_H
#define EMBEDDEDSHELLINTEGRATION_H
#include "qwayland-embedded-shell.h"
#include <QtWaylandClient/private/qwaylandclientextension_p.h>
#include <QtWaylandClient/private/qwaylandshellintegration_p.h>

using namespace QtWaylandClient;
class EmbeddedShellSurface;

class Q_WAYLAND_CLIENT_EXPORT EmbeddedShellIntegration
    : public QWaylandShellIntegration,
      public QWaylandClientExtension,
      public QtWayland::embedded_shell {
  QMap<QWaylandWindow *, EmbeddedShellSurface *> m_windows;

public:
  EmbeddedShellIntegration();
  QWaylandShellSurface *createShellSurface(QWaylandWindow *window) override;
  bool initialize(QWaylandDisplay *display) override;
  const struct wl_interface *extensionInterface() const override;
  void bind(struct ::wl_registry *registry, int id, int ver) override;

  // QWaylandShellIntegration interface
  void *nativeResourceForWindow(const QByteArray &resource,
                                QWindow *window) override;
};

#endif // EMBEDDEDSHELLINTEGRATION_H
