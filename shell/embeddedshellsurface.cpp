#include "embeddedshellsurface.h"
#include <qpa/qwindowsysteminterface.h>
#include <qpa/qplatformwindow.h>
#include<QtWaylandClient/private/qwaylandwindow_p.h>

EmbeddedShellSurface::EmbeddedShellSurface(struct ::embedded_shell_surface *shell_surface, QWaylandWindow *window, embedded_shell_anchor_border anchor)
    : QWaylandShellSurface(window)
    , QtWayland::embedded_shell_surface(shell_surface)
    , m_anchor(anchor)
{
    qDebug()<<__PRETTY_FUNCTION__<<anchor;
}

EmbeddedShellSurface::~EmbeddedShellSurface()
{
}


/*
void EmbeddedShellSurface::applyConfigure()
{
    if (m_stateChanged)
        QWindowSystemInterface::handleWindowStateChanged(window()->window(), m_pendingStates);
    m_stateChanged = false;
}
*/

void EmbeddedShellSurface::embedded_shell_surface_minimize(uint32_t minimized)
{
    m_pendingStates = minimized ? Qt::WindowMinimized : Qt::WindowNoState;
    m_stateChanged = true;
    window()->applyConfigureWhenPossible();
}
