// SPDX-License-Identifier: GPL-3.0-only

#include "embeddedshellextension.h"
#include "qwayland-server-embedded-shell.h"
#include <QWaylandResource>
#include <QWaylandSurface>
#include <QtWaylandCompositor/QWaylandResource>
#include <QtWaylandCompositor/private/qwaylandutils_p.h>

Q_LOGGING_CATEGORY(shellExt, "embeddedshell.compositor")

namespace
{
QVariantMap array_to_variant_map(wl_array *custom_data)
{
  QVariantMap result;

  if (custom_data) {
    auto byteArray = QByteArray(static_cast<const char *>(custom_data->data), custom_data->size);
    QDataStream stream(byteArray);
    stream.setVersion(QDataStream::Qt_6_8);

    stream >> result;

    if (stream.status() != QDataStream::Status::Ok) {
      qCWarning(shellExt) << Q_FUNC_INFO << "Could not deserialize" << byteArray << "!";
      result.clear();
    }
  }

  return result;
};
}

EmbeddedShellExtension::EmbeddedShellExtension(QWaylandCompositor *compositor)
    : QWaylandShellTemplate<EmbeddedShellExtension>(compositor)
{
  qCDebug(shellExt) << Q_FUNC_INFO << compositor;
}

EmbeddedShellExtension::EmbeddedShellExtension()
    : QWaylandShellTemplate<EmbeddedShellExtension>()
{
  qCDebug(shellExt) << Q_FUNC_INFO;
}

void EmbeddedShellExtension::initialize()
{
  qCDebug(shellExt) << Q_FUNC_INFO;
  QWaylandShellTemplate::initialize();

  QWaylandCompositor *compositor =
      static_cast<QWaylandCompositor *>(extensionContainer());
  if (!compositor) {
    qWarning() << "Failed to find QWaylandCompositor when initializing "
                  "QWaylandXdgShell";
    return;
  }
  init(compositor->display(), 1);
}

void EmbeddedShellExtension::embedded_shell_surface_create(Resource *resource,
                                                           wl_resource *wl_surface,
                                                           uint32_t id,
                                                           uint32_t anchor,
                                                           uint32_t margin)
{
  qCDebug(shellExt) << Q_FUNC_INFO << id << "anchor" << anchor << "margin" << margin;
  Q_UNUSED(resource)

  QWaylandSurface *surface = QWaylandSurface::fromResource(wl_surface);

  QWaylandResource embeddedShellSurfaceResource(wl_resource_create(resource->client(),
                                                                   &embedded_shell_surface_interface,
                                                                   wl_resource_get_version(resource->handle),
                                                                   id));
  auto embeddedShellSurface = QtWayland::fromResource<EmbeddedShellSurface *>(embeddedShellSurfaceResource.resource());

  if (!embeddedShellSurface) {
    qCDebug(shellExt) << "server received new surface" << surface << anchor;
    embeddedShellSurface = new EmbeddedShellSurface(this,
                                                    surface,
                                                    embeddedShellSurfaceResource,
                                                    static_cast<EmbeddedShellTypes::Anchor>(anchor),
                                                    margin);
  } else {
    qCDebug(shellExt) << "server received already known surface" << surface
                      << embeddedShellSurface;
  }

  emit surfaceAdded(embeddedShellSurface);
}

EmbeddedShellSurface::EmbeddedShellSurface(EmbeddedShellExtension *extension,
                                           QWaylandSurface *surface,
                                           const QWaylandResource &resource,
                                           EmbeddedShellTypes::Anchor anchor,
                                           uint32_t margin)
    : QWaylandShellSurfaceTemplate<EmbeddedShellSurface>(this)
    , m_surface(surface)
    , m_size(QSize{0, 0})
    , m_anchor(anchor)
    , m_margin(margin)
{
  Q_UNUSED(extension)
  qCDebug(shellExt) << Q_FUNC_INFO << anchor
                    << wl_resource_get_id(resource.resource());
  init(resource.resource());
  setExtensionContainer(surface);
  QWaylandCompositorExtension::initialize();
}

QWaylandQuickShellIntegration *EmbeddedShellSurface::createIntegration(QWaylandQuickShellSurfaceItem *item)
{
  qCDebug(shellExt) << Q_FUNC_INFO;
  return new QuickEmbeddedShellIntegration(item);
}

QWaylandSurface *EmbeddedShellSurface::surface() const
{
  return m_surface;
}

EmbeddedShellTypes::Anchor EmbeddedShellSurface::anchor()
{
  return m_anchor;
}

int EmbeddedShellSurface::margin()
{
  return m_margin;
}

QSize EmbeddedShellSurface::size() const
{
  return m_size;
}

QString EmbeddedShellSurface::uuid() const
{
  return m_uuid.toString(QUuid::WithoutBraces);
}

void EmbeddedShellSurface::sendConfigure(const QSize size)
{
  qCDebug(shellExt) << Q_FUNC_INFO << size;
  send_configure(size.width(), size.height());
}

void EmbeddedShellSurface::sendVisibleChanged(bool visible)
{
  qCDebug(shellExt) << Q_FUNC_INFO << visible;
  send_visible_changed(visible);
}

pid_t EmbeddedShellSurface::getClientPid() const
{
  pid_t pid;
  uid_t uid;
  gid_t gid;
  wl_client_get_credentials(resource()->client(), &pid, &uid, &gid);
  return pid;
}

void EmbeddedShellSurface::updateAnchor(EmbeddedShellTypes::Anchor newAnchor)
{
  qCDebug(shellExt) << Q_FUNC_INFO << newAnchor;
  m_anchor = newAnchor;
  emit anchorChanged(newAnchor);
}

void EmbeddedShellSurface::updateMargin(int newMargin)
{
  qCDebug(shellExt) << Q_FUNC_INFO << newMargin;
  m_margin = newMargin;
  emit marginChanged(newMargin);
}

void EmbeddedShellSurface::updateSize(const QSize &size)
{
  qCDebug(shellExt) << Q_FUNC_INFO << m_size << "->" << size;
  if (m_size != size) {
    m_size = size;
    emit sizeChanged(size);
  }
}

void EmbeddedShellSurface::embedded_shell_surface_set_anchor(Resource *resource,
                                                             uint32_t anchor)
{
  Q_UNUSED(resource)
  auto newAnchor = static_cast<EmbeddedShellTypes::Anchor>(anchor);
  updateAnchor(newAnchor);
}

void EmbeddedShellSurface::embedded_shell_surface_set_margin(Resource *resource,
                                                             int32_t margin)
{
  qCDebug(shellExt) << Q_FUNC_INFO << margin;
  Q_UNUSED(resource)
  updateMargin(margin);
}

void EmbeddedShellSurface::embedded_shell_surface_set_size(Resource *resource,
                                                           uint32_t width,
                                                           uint32_t height)
{
  Q_UNUSED(resource)
  updateSize(QSize(width, height));
}

void EmbeddedShellSurface::embedded_shell_surface_view_create(Resource *resource,
                                                              wl_resource *shell_surface,
                                                              const QString &label,
                                                              const QString &icon,
                                                              unsigned int sort_index,
                                                              wl_array *custom_data,
                                                              wl_resource *parent_view,
                                                              uint32_t id)
{
  Q_UNUSED(shell_surface)
  EmbeddedShellSurfaceView *parentView = nullptr;

  if (parent_view) {
    auto surfaceViewResource = QtWaylandServer::surface_view::Resource::fromResource(parent_view);
    Q_ASSERT(surfaceViewResource);

    auto waylandSurfaceView = surfaceViewResource->object();
    Q_ASSERT(waylandSurfaceView);

    parentView = dynamic_cast<EmbeddedShellSurfaceView *>(waylandSurfaceView);
    Q_ASSERT(parentView);
  }

  auto customDataVariant = array_to_variant_map(custom_data);
  qCDebug(shellExt) << Q_FUNC_INFO << label << icon << id;
  auto view = new EmbeddedShellSurfaceView(label,
                                           icon,
                                           sort_index,
                                           customDataVariant,
                                           resource->client(),
                                           parentView,
                                           id,
                                           1);
  emit createView(view);
}

EmbeddedShellSurfaceView::EmbeddedShellSurfaceView(const QString &label,
                                                   const QString &icon,
                                                   uint32_t sortIndex,
                                                   const QVariantMap &customData,
                                                   wl_client *client,
                                                   EmbeddedShellSurfaceView *parentView,
                                                   int id,
                                                   int version)
    : QtWaylandServer::surface_view(client, id, version)
    , m_label(label)
    , m_icon(icon)
    , m_sortIndex(sortIndex)
    , m_customData(customData)
    , m_parentView(parentView)
{
}

QString EmbeddedShellSurfaceView::label() const
{
    return m_label;
}

QString EmbeddedShellSurfaceView::icon() const
{
  return m_icon;
}

unsigned int EmbeddedShellSurfaceView::sortIndex() const
{
  return m_sortIndex;
}

QVariantMap EmbeddedShellSurfaceView::customData() const
{
  return m_customData;
}

QString EmbeddedShellSurfaceView::parentUuid() const
{
  return m_parentView ? m_parentView->uuid() : QString();
}

QString EmbeddedShellSurfaceView::uuid() const
{
  return m_uuid.toString(QUuid::WithoutBraces);
}

void EmbeddedShellSurfaceView::select()
{
  surface_view::send_selected();
}

void EmbeddedShellSurfaceView::updateLabel(const QString &label)
{
    if (m_label == label) {
        return;
    }

    m_label = label;
    emit labelChanged(label);
}



void EmbeddedShellSurfaceView::updateIcon(const QString &icon)
{
    if (m_icon == icon) {
        return;
    }

    m_icon = icon;
    emit iconChanged(icon);
}

void EmbeddedShellSurfaceView::updateSortIndex(unsigned int newSortIndex)
{
  if (m_sortIndex == newSortIndex)
    return;
  m_sortIndex = newSortIndex;
  emit sortIndexChanged(m_sortIndex);
}



void EmbeddedShellSurfaceView::updateCustomData(const QVariantMap &customData)
{
  if (m_customData == customData)
    return;
  m_customData = customData;
  emit customDataChanged(m_customData);
}

void EmbeddedShellSurfaceView::surface_view_set_label(Resource *resource,
                                                      const QString &text)
{
    qCDebug(shellExt) << Q_FUNC_INFO << text;
    Q_UNUSED(resource)
    updateLabel(text);
}

void EmbeddedShellSurfaceView::surface_view_set_icon(Resource *resource,
                                                     const QString &icon)
{
    qCDebug(shellExt) << Q_FUNC_INFO << icon;
    Q_UNUSED(resource)
    updateIcon(icon);
}

void EmbeddedShellSurfaceView::surface_view_set_sort_index(Resource *resource,
                                                           uint32_t sort_index)
{
  Q_UNUSED(resource)
  updateSortIndex(sort_index);
}

void EmbeddedShellSurfaceView::surface_view_set_custom_data(Resource *resource,
                                                            wl_array *custom_data)
{
  Q_UNUSED(resource)
  updateCustomData(array_to_variant_map(custom_data));
}

void EmbeddedShellSurfaceView::surface_view_select(Resource *resource)
{
  Q_UNUSED(resource)
  emit aboutToBeSelected();
  QtWaylandServer::surface_view::surface_view_select(resource);
}

void EmbeddedShellSurfaceView::surface_view_destroy(Resource *resource)
{
  Q_UNUSED(resource)
  emit aboutToBeDestroyed();
  QtWaylandServer::surface_view::surface_view_destroy(resource);
  deleteLater();
}

QuickEmbeddedShellIntegration::QuickEmbeddedShellIntegration(QWaylandQuickShellSurfaceItem *item)
    : QWaylandQuickShellIntegration(item), m_item(item)
    , m_shellSurface(qobject_cast<EmbeddedShellSurface *>(item->shellSurface()))
{
  qCDebug(shellExt) << Q_FUNC_INFO;
  m_item->setSurface(m_shellSurface->surface());
  connect(m_shellSurface, &EmbeddedShellSurface::destroyed, this,
          &QuickEmbeddedShellIntegration::handleEmbeddedShellSurfaceDestroyed);
}

QuickEmbeddedShellIntegration::~QuickEmbeddedShellIntegration()
{
  qCDebug(shellExt) << Q_FUNC_INFO;
  m_item->setSurface(nullptr);
}

EmbeddedShellTypes::Anchor QuickEmbeddedShellIntegration::getAnchor()
{
  return m_shellSurface->anchor();
}

int QuickEmbeddedShellIntegration::getMargin()
{
  return m_shellSurface->margin();
}

void QuickEmbeddedShellIntegration::sendConfigure(const QSize size)
{
  m_shellSurface->send_configure(size.width(), size.height());
}

void QuickEmbeddedShellIntegration::handleEmbeddedShellSurfaceDestroyed()
{
  qCDebug(shellExt) << Q_FUNC_INFO;
  m_shellSurface = nullptr;
}