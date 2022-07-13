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
    void applyConfigure() override;

public slots:
    void sendAnchor(embedded_shell_anchor_border anchor);

protected:
//    void embedded_shell_surface_minimize(uint32_t minimized) override;

private:
//    Qt::WindowStates m_pendingStates;
//    bool m_stateChanged = false;
    embedded_shell_anchor_border m_anchor;
    QSize m_pendingSize = {0,0};
    QWaylandWindow *m_window = nullptr;
protected:
    void embedded_shell_surface_configure(int32_t width, int32_t height) override;
    void embedded_shell_surface_switch_view(uint32_t view_index) override;
};

#endif // EMBEDDEDSHELLSURFACE_H
