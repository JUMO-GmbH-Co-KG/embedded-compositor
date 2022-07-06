#ifndef EMBEDDEDSHELLEXTENSION_H
#define EMBEDDEDSHELLEXTENSION_H

#include <QtWaylandCompositor/QWaylandShellSurfaceTemplate>
#include <QtWaylandCompositor/QWaylandShellTemplate>

#include <QtWaylandCompositor/QWaylandQuickExtension>
#include <QtWaylandCompositor/QWaylandCompositor>
#include <QWaylandResource>
#include "qwayland-server-embedded-shell.h"


#include <QtWaylandCompositor/QWaylandQuickShellIntegration>
#include <QtWaylandCompositor/QWaylandQuickShellSurfaceItem>

class EmbeddedShellSurface;

class EmbeddedShellExtension
        : public QWaylandShellTemplate<EmbeddedShellExtension>
        , public QtWaylandServer::embedded_shell
{
    Q_OBJECT
public:
    explicit EmbeddedShellExtension(QWaylandCompositor *compositor);
    explicit EmbeddedShellExtension();
    void embedded_shell_surface_create(Resource *resource, struct ::wl_resource *surface, uint32_t id, uint32_t anchor) override;

//    const wl_interface *extensionInterface() const override;
    void initialize() override;
signals:
    void surfaceAdded(EmbeddedShellSurface *surface);
};

class EmbeddedShellSurface
        : public QWaylandShellSurfaceTemplate<EmbeddedShellSurface>
        , public QtWaylandServer::embedded_shell_surface
{
    Q_OBJECT
public:
    EmbeddedShellSurface(EmbeddedShellExtension *ext, QWaylandSurface *surface, const QWaylandResource &resource, embedded_shell_anchor_border anchor);
    QWaylandQuickShellIntegration *createIntegration(QWaylandQuickShellSurfaceItem *item) override;
    QWaylandSurface *surface() const { return m_surface; }
    embedded_shell_anchor_border getAnchor() { return m_anchor; }
    Q_PROPERTY(uint anchor READ getAnchor)
private:
    QWaylandSurface* m_surface;
    embedded_shell_anchor_border m_anchor;
};



class QuickEmbeddedShellIntegration : public  QWaylandQuickShellIntegration
{
    Q_OBJECT
public:
    QuickEmbeddedShellIntegration(QWaylandQuickShellSurfaceItem *item);
    ~QuickEmbeddedShellIntegration() override;
    Q_PROPERTY(uint anchor READ getAnchor)
    uint getAnchor() { return m_shellSurface->getAnchor(); }

private slots:
    void handleEmbeddedShellSurfaceDestroyed();

private:
    QWaylandQuickShellSurfaceItem *m_item = nullptr;
    EmbeddedShellSurface *m_shellSurface = nullptr;
};


Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(EmbeddedShellExtension)

#endif // EMBEDDEDSHELLEXTENSION_H
