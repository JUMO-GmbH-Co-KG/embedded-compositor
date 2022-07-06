#ifndef EMBEDDEDSHELLINTEGRATION_H
#define EMBEDDEDSHELLINTEGRATION_H
#include <QtWaylandClient/private/qwaylandshellintegration_p.h>
#include <QtWaylandClient/private/qwaylandclientextension_p.h>
#include "qwayland-embedded-shell.h"

using namespace QtWaylandClient;

class Q_WAYLAND_CLIENT_EXPORT EmbeddedShellIntegration
        : public QWaylandShellIntegration
        , public QWaylandClientExtension
        , public QtWayland::embedded_shell
{
public:
    EmbeddedShellIntegration();
    QWaylandShellSurface *createShellSurface(QWaylandWindow *window) override;
    bool initialize(QWaylandDisplay *display) override;
    const struct wl_interface *extensionInterface() const override;
    void bind(struct ::wl_registry *registry, int id, int ver) override;
};

#endif // EMBEDDEDSHELLINTEGRATION_H
