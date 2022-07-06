#ifndef EMBEDDEDSHELLSURFACE_H
#define EMBEDDEDSHELLSURFACE_H

//#include <QtWaylandClient/private/qwaylandclientshellapi_p.h>
//#include <QtWaylandClient/private/qwaylandshellintegration_p.h>
#include <QtWaylandClient/private/qwaylandshellsurface_p.h>
#include "qwayland-embedded-shell.h"

using namespace QtWaylandClient;

class EmbeddedShellSurface : public QWaylandShellSurface
        , public QtWayland::embedded_shell_surface
{
public:
    EmbeddedShellSurface(struct ::embedded_shell_surface *shell_surface, QWaylandWindow *window, embedded_shell_anchor_border anchor);
    ~EmbeddedShellSurface() override;

    int getAnchor() const {
        return m_anchor;
    }
//    void applyConfigure() override;

protected:
    void embedded_shell_surface_minimize(uint32_t minimized) override;

private:
    Qt::WindowStates m_pendingStates;
    bool m_stateChanged = false;
    embedded_shell_anchor_border m_anchor;
};

#endif // EMBEDDEDSHELLSURFACE_H
