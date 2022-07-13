#include "embeddedshellintegration.h"
#include "embeddedshellsurface.h"
#include "QtWaylandClient/private/qwaylandwindow_p.h"
#include "quickembeddedshellwindow.h"

EmbeddedShellIntegration::EmbeddedShellIntegration()
    : QWaylandClientExtension(1)

{
    qDebug()<<__PRETTY_FUNCTION__<<isActive();

}
QWaylandShellSurface *EmbeddedShellIntegration::createShellSurface(QWaylandWindow *window)
{
    qDebug()<<__PRETTY_FUNCTION__<<isActive();
    if (!isActive())
        return nullptr;
    embedded_shell_anchor_border anchor = embedded_shell_anchor_border::EMBEDDED_SHELL_ANCHOR_BORDER_UNDEFINED;

    QuickEmbeddedShellWindow* qew =  qobject_cast<QuickEmbeddedShellWindow*>(window->window());
    if(qew != nullptr) {
        anchor = static_cast<embedded_shell_anchor_border>(qew->anchor());
    }
    auto *surface = surface_create(window->wlSurface(), anchor);
    auto ess = new EmbeddedShellSurface(surface, window, anchor);

    if(qew != nullptr) {
        connect(qew, &QuickEmbeddedShellWindow::anchorChanged, ess,
                [ess](auto anchor) {
            qDebug()<<"sending anchor"<<anchor;
            ess->sendAnchor(static_cast<embedded_shell_anchor_border>(anchor));
        });
    }
    return ess;
}

bool EmbeddedShellIntegration::initialize(QWaylandDisplay *display)
{
    QWaylandShellIntegration::initialize(display);
    qDebug()<<isActive();
    return isActive();
}

const wl_interface *EmbeddedShellIntegration::extensionInterface() const
{
    return QtWayland::embedded_shell::interface();
}

void EmbeddedShellIntegration::bind(wl_registry *registry, int id, int ver)
{
    auto* instance = static_cast<QtWayland::embedded_shell *>(this);
    // Make sure lowest version is used of the supplied version from the
    // developer and the version specified in the protocol and also the
    // compositor version.
    if (this->version() > QtWayland::embedded_shell::interface()->version) {
        qWarning("Supplied protocol version to QWaylandClientExtensionTemplate is higher than the version of the protocol, using protocol version instead.");
    }
    int minVersion = qMin(ver, qMin(QtWayland::embedded_shell::interface()->version, this->version()));
    setVersion(minVersion);
    instance->init(registry, id, minVersion);
}
