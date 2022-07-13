#include "embeddedshellextension.h"
#include <QWaylandSurface>
#include <QtWaylandCompositor/QWaylandResource>
#include <QtWaylandCompositor/private/qwaylandutils_p.h>
#include "qwayland-server-embedded-shell.h"
#include <QWaylandResource>


EmbeddedShellExtension::EmbeddedShellExtension(QWaylandCompositor *compositor)
    : QWaylandShellTemplate<EmbeddedShellExtension>(compositor)
{
    qDebug()<<__PRETTY_FUNCTION__<<compositor;
}

EmbeddedShellExtension::EmbeddedShellExtension()
    : QWaylandShellTemplate<EmbeddedShellExtension>()
{
    qDebug()<<__PRETTY_FUNCTION__;
}

void EmbeddedShellExtension::initialize()
{
    qDebug()<<__PRETTY_FUNCTION__;
    QWaylandShellTemplate::initialize();

    QWaylandCompositor *compositor = static_cast<QWaylandCompositor *>(extensionContainer());
    if (!compositor) {
        qWarning() << "Failed to find QWaylandCompositor when initializing QWaylandXdgShell";
        return;
    }
    init(compositor->display(), 1);
}


void EmbeddedShellExtension::embedded_shell_surface_create(Resource *resource, wl_resource *wl_surface, uint32_t id, uint32_t anchor)
{
    qDebug()<<__PRETTY_FUNCTION__<<id<<anchor;
    Q_UNUSED(resource)

    QWaylandSurface *surface = QWaylandSurface::fromResource(wl_surface);

    QWaylandResource embeddedShellSurfaceResource(
                wl_resource_create(resource->client(),
                                   &embedded_shell_surface_interface,
                                   wl_resource_get_version(resource->handle),
                                   id)
                );
    auto embeddedShellSurface = QtWayland::fromResource<EmbeddedShellSurface*>(embeddedShellSurfaceResource.resource());

    if (!embeddedShellSurface) {
        qDebug() << "server received new surface" << surface<<anchor;
        embeddedShellSurface = new EmbeddedShellSurface(this, surface, embeddedShellSurfaceResource, static_cast<embedded_shell_anchor_border>(anchor));
    } else {
        qDebug() << "server received already known surface" << surface<<embeddedShellSurface;
    }

    emit surfaceAdded(embeddedShellSurface);
}

EmbeddedShellSurface::EmbeddedShellSurface(EmbeddedShellExtension *ext, QWaylandSurface *surface, const QWaylandResource &resource, embedded_shell_anchor_border anchor)
    : QWaylandShellSurfaceTemplate<EmbeddedShellSurface>(this)
    , m_surface(surface)
    , m_anchor(anchor)
{
    qDebug()<<__PRETTY_FUNCTION__<<anchor;
    init(resource.resource());
    setExtensionContainer(surface);
    QWaylandCompositorExtension::initialize();
}

QWaylandQuickShellIntegration *EmbeddedShellSurface::createIntegration(QWaylandQuickShellSurfaceItem *item)
{
    qDebug()<<__PRETTY_FUNCTION__;
    return new QuickEmbeddedShellIntegration(item);
}

void EmbeddedShellSurface::sendConfigure(const QSize size)
{
    qDebug()<<__PRETTY_FUNCTION__<<size;
    send_configure(size.width(), size.height());
}

QuickEmbeddedShellIntegration::QuickEmbeddedShellIntegration(QWaylandQuickShellSurfaceItem *item)
    : QWaylandQuickShellIntegration(item)
    , m_item(item)
    , m_shellSurface(qobject_cast<EmbeddedShellSurface *>(item->shellSurface()))
{
    qDebug()<<__PRETTY_FUNCTION__;
    m_item->setSurface(m_shellSurface->surface());
    connect(m_shellSurface, &EmbeddedShellSurface::destroyed, this, &QuickEmbeddedShellIntegration::handleEmbeddedShellSurfaceDestroyed);
}

QuickEmbeddedShellIntegration::~QuickEmbeddedShellIntegration()
{
    qDebug()<<__PRETTY_FUNCTION__;
    m_item->setSurface(nullptr);
}

void QuickEmbeddedShellIntegration::sendConfigure(const QSize size)
{
    m_shellSurface->send_configure(size.width(), size.height());
}

void QuickEmbeddedShellIntegration::handleEmbeddedShellSurfaceDestroyed()
{
    qDebug()<<__PRETTY_FUNCTION__;
    m_shellSurface = nullptr;
}

void EmbeddedShellSurface::embedded_shell_surface_set_anchor(Resource *resource, uint32_t anchor)
{
    Q_UNUSED(resource)
    qDebug()<<__PRETTY_FUNCTION__<<m_anchor<<"->"<<anchor;
    m_anchor = static_cast<embedded_shell_anchor_border>(anchor);
    emit anchorChanged(m_anchor);
}
