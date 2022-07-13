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

void EmbeddedShellSurface::embedded_shell_surface_configure(int32_t width, int32_t height)
{
    qDebug()<<__PRETTY_FUNCTION__<<width<<height;
    m_pendingSize = {width, height};
    window()->applyConfigureWhenPossible();
}

void EmbeddedShellSurface::embedded_shell_surface_switch_view(uint32_t view_index)
{
    qDebug()<<__PRETTY_FUNCTION__<<view_index<<"TODO";
}

void EmbeddedShellSurface::applyConfigure()
{
    qDebug()<<__PRETTY_FUNCTION__;
    window()->resizeFromApplyConfigure(m_pendingSize);
}

void EmbeddedShellSurface::sendAnchor(embedded_shell_anchor_border anchor)
{
    qDebug()<<__PRETTY_FUNCTION__<<anchor;
    set_anchor(anchor);
}

/*
void EmbeddedShellSurface::embedded_shell_surface_minimize(uint32_t minimized)
{
    m_pendingStates = minimized ? Qt::WindowMinimized : Qt::WindowNoState;
    m_stateChanged = true;
    window()->applyConfigureWhenPossible();
}
*/
