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
QVariant array_to_variant(wl_array *custom_data)
{
  QVariant result;

  if (custom_data) {
    auto byteArray = QByteArray(static_cast<const char *>(custom_data->data), custom_data->size);
    QDataStream stream(byteArray);
    stream.setVersion(QDataStream::Qt_6_8);

    stream >> result;

    if (stream.status() != QDataStream::Status::Ok) {
      qCWarning(shellExt) << Q_FUNC_INFO << "Could not deserialize" << byteArray << "!";
      result = QVariant();
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

void EmbeddedShellExtension::embedded_shell_surface_create(
    Resource *resource,
    wl_resource *wl_surface,
    uint32_t id,
    uint32_t anchor,
    uint32_t margin,
    unsigned int sort_index)
{
  qCDebug(shellExt) << Q_FUNC_INFO << id << "anchor" << anchor
                    << "margin" << margin;
  Q_UNUSED(resource)

  QWaylandSurface *surface = QWaylandSurface::fromResource(wl_surface);

  QWaylandResource embeddedShellSurfaceResource(
      wl_resource_create(resource->client(),
                         &embedded_shell_surface_interface,
                         wl_resource_get_version(resource->handle),
                         id));
  auto embeddedShellSurface = QtWayland::fromResource<EmbeddedShellSurface *>(
      embeddedShellSurfaceResource.resource());

  if (!embeddedShellSurface) {
    qCDebug(shellExt) << "server received new surface" << surface << anchor;
    embeddedShellSurface = new EmbeddedShellSurface(
        this, surface, embeddedShellSurfaceResource,
        static_cast<EmbeddedShellTypes::Anchor>(anchor), margin, sort_index);
  } else {
    qCDebug(shellExt) << "server received already known surface" << surface
                      << embeddedShellSurface;
  }

  emit surfaceAdded(embeddedShellSurface);
}

EmbeddedShellSurface::EmbeddedShellSurface(EmbeddedShellExtension *ext,
                                           QWaylandSurface *surface,
                                           const QWaylandResource &resource,
                                           EmbeddedShellTypes::Anchor anchor,
                                           uint32_t margin, uint32_t sort_index)
    : QWaylandShellSurfaceTemplate<EmbeddedShellSurface>(this),
      m_surface(surface), m_size(QSize{0, 0}), m_anchor(anchor), m_margin(margin),
      m_sort_index(sort_index)
{
  Q_UNUSED(ext)
  qCDebug(shellExt) << Q_FUNC_INFO << anchor
                    << wl_resource_get_id(resource.resource());
  init(resource.resource());
  setExtensionContainer(surface);
  QWaylandCompositorExtension::initialize();
}

QWaylandQuickShellIntegration *
EmbeddedShellSurface::createIntegration(QWaylandQuickShellSurfaceItem *item)
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

void EmbeddedShellSurface::setSize(const QSize &size)
{
    qCDebug(shellExt) << Q_FUNC_INFO << m_size << "->" << size;
    if (m_size != size) {
        m_size = size;
        emit sizeChanged(size);
    }
}

QVariant EmbeddedShellSurface::customData() const
{
  return m_customData;
}

void EmbeddedShellSurface::setCustomData(const QVariant &customData)
{
  qCDebug(shellExt) << Q_FUNC_INFO << m_customData << "->" << customData;
  if (m_customData != customData) {
    m_customData = customData;
    emit customDataChanged(customData);
  }
}

void EmbeddedShellSurface::setMargin(int newMargin)
{
  qCDebug(shellExt) << Q_FUNC_INFO << newMargin;
  m_margin = newMargin;
  emit marginChanged(newMargin);
}

unsigned int EmbeddedShellSurface::sortIndex()
{
  return m_sort_index;
}

void EmbeddedShellSurface::setSortIndex(unsigned int sort_index)
{
  m_sort_index = sort_index;
  emit sortIndexChanged(sort_index);
}

QString EmbeddedShellSurface::appId() const
{
  return m_appId;
}

void EmbeddedShellSurface::setAppId(const QString &appId)
{
  if (!m_appId.isEmpty()) {
    return;
  }
  m_appId = appId;
}

QString EmbeddedShellSurface::appLabel() const
{
  return m_appLabel;
}

void EmbeddedShellSurface::setAppLabel(const QString &appLabel)
{
  if (m_appLabel != appLabel) {
    m_appLabel = appLabel;
    emit appLabelChanged(appLabel);
  }
}

QString EmbeddedShellSurface::appIcon() const
{
  return m_appIcon;
}

void EmbeddedShellSurface::setAppIcon(const QString &appIcon)
{
  if (m_appIcon != appIcon) {
    m_appIcon = appIcon;
    emit appIconChanged(appIcon);
  }
}

QSize EmbeddedShellSurface::size() const
{
  return m_size;
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

QString EmbeddedShellSurface::uuid() const
{
  return m_uuid.toString(QUuid::WithoutBraces);
}

pid_t EmbeddedShellSurface::getClientPid() const
{
  pid_t pid;
  uid_t uid;
  gid_t gid;
  wl_client_get_credentials(resource()->client(), &pid, &uid, &gid);
  return pid;
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

void EmbeddedShellSurface::embedded_shell_surface_set_size(Resource *resource,
                                                           uint32_t width,
                                                           uint32_t height)
{
    Q_UNUSED(resource)
    setSize(QSize(width, height));
}

void EmbeddedShellSurface::embedded_shell_surface_set_custom_data(Resource *resource,
                                                                  wl_array *custom_data)
{
  Q_UNUSED(resource)
  setCustomData(array_to_variant(custom_data));
}

void EmbeddedShellSurface::embedded_shell_surface_set_anchor(Resource *resource,
                                                             uint32_t anchor)
{
  Q_UNUSED(resource)
  auto newAnchor = static_cast<EmbeddedShellTypes::Anchor>(anchor);
  qCDebug(shellExt) << Q_FUNC_INFO << m_anchor << "->" << newAnchor;
  if(newAnchor != m_anchor) {
    m_anchor = newAnchor;
    emit anchorChanged(m_anchor);
  }
}

void EmbeddedShellSurface::embedded_shell_surface_view_create(Resource *resource,
                                                              wl_resource *shell_surface,
                                                              const QString &appId,
                                                              const QString &appLabel,
                                                              const QString &appIcon,
                                                              const QString &label,
                                                              const QString &icon,
                                                              uint32_t sort_index,
                                                              wl_array *custom_data,
                                                              uint32_t id)
{
  Q_UNUSED(shell_surface)
  auto customDataVariant = array_to_variant(custom_data);
  qCDebug(shellExt) << Q_FUNC_INFO << appId << appLabel << appIcon << label << icon << id << customDataVariant;
  auto view = new EmbeddedShellSurfaceView(appId,
                                           appLabel,
                                           appIcon,
                                           label,
                                           icon,
                                           sort_index,
                                           customDataVariant,
                                           resource->client(),
                                           id,
                                           1);
  emit createView(view);
}

EmbeddedShellSurfaceView::EmbeddedShellSurfaceView(const QString &appId,
                                                   const QString &label,
                                                   const QString &icon,
                                                   uint32_t sort_index,
                                                   wl_client *client,
                                                   int id,
                                                   int version)
    : QtWaylandServer::surface_view(client, id, version)
    , m_appId(appId)
    , m_label(label)
    , m_icon(icon)
    , m_sortIndex(sort_index)
{
}

EmbeddedShellSurfaceView::EmbeddedShellSurfaceView(const QString &appId,
                                                   const QString &appLabel,
                                                   const QString &appIcon,
                                                   const QString &label,
                                                   const QString &icon,
                                                   uint32_t sort_index,
                                                   const QVariant &custom_data,
                                                   wl_client *client,
                                                   int id,
                                                   int version)
    : QtWaylandServer::surface_view(client, id, version)
    , m_appId(appId)
    , m_appLabel(appLabel)
    , m_appIcon(appIcon)
    , m_label(label)
    , m_icon(icon)
    , m_sortIndex(sort_index)
    , m_customData(custom_data)
{
}

QString EmbeddedShellSurfaceView::appId() const
{
    return m_appId;
}

void EmbeddedShellSurfaceView::setAppId(const QString &appId)
{
    if (m_appId == appId) {
        return;
    }

    // TODO prevent changing after it was initialized.
    m_appId = appId;
    emit appIdChanged(appId);
}

QString EmbeddedShellSurfaceView::appLabel() const
{
    return m_appLabel;
}

void EmbeddedShellSurfaceView::setAppLabel(const QString &appLabel)
{
    if (m_appLabel == appLabel) {
        return;
    }

    m_appLabel = appLabel;
    emit appLabelChanged(appLabel);
}

QString EmbeddedShellSurfaceView::appIcon() const
{
    return m_appIcon;
}

void EmbeddedShellSurfaceView::setAppIcon(const QString &appIcon)
{
    if (m_appIcon == appIcon) {
        return;
    }

    m_appIcon = appIcon;
    emit appIconChanged(appIcon);
}

QString EmbeddedShellSurfaceView::label() const
{
    return m_label;
}

void EmbeddedShellSurfaceView::setLabel(const QString &label)
{
    if (m_label == label) {
        return;
    }

    m_label = label;
    emit labelChanged(label);
}

QString EmbeddedShellSurfaceView::icon() const
{
    return m_icon;
}

void EmbeddedShellSurfaceView::setIcon(const QString &icon)
{
    if (m_icon == icon) {
        return;
    }

    m_icon = icon;
    emit iconChanged(icon);
}

void EmbeddedShellSurfaceView::surface_view_set_app_label(Resource *resource,
                                                          const QString &label)
{
    qCDebug(shellExt) << Q_FUNC_INFO << label;
    Q_UNUSED(resource);
    setAppLabel(label);
}

void EmbeddedShellSurfaceView::surface_view_set_app_icon(Resource *resource,
                                                         const QString &icon)
{
    qCDebug(shellExt) << Q_FUNC_INFO << icon;
    Q_UNUSED(resource);
    setAppIcon(icon);
}

void EmbeddedShellSurfaceView::surface_view_set_label(Resource *resource,
                                                      const QString &text)
{
    qCDebug(shellExt) << Q_FUNC_INFO << text;
    Q_UNUSED(resource)
    setLabel(text);
}

void EmbeddedShellSurfaceView::surface_view_set_icon(Resource *resource,
                                                     const QString &icon)
{
    qCDebug(shellExt) << Q_FUNC_INFO << icon;
    Q_UNUSED(resource)
    setIcon(icon);
}

void EmbeddedShellSurfaceView::surface_view_set_sort_index(Resource *resource,
                                                           uint32_t sort_index)
{
  Q_UNUSED(resource)
  setSortIndex(sort_index);
}

void EmbeddedShellSurfaceView::surface_view_set_custom_data(Resource *resource,
                                                            wl_array *custom_data)
{
  Q_UNUSED(resource)
  setCustomData(array_to_variant(custom_data));
}

void EmbeddedShellSurfaceView::surface_view_destroy(Resource *resource)
{
  Q_UNUSED(resource)
  emit aboutToBeDestroyed();
  QtWaylandServer::surface_view::surface_view_destroy(resource);
  deleteLater();
}

void EmbeddedShellSurface::embedded_shell_surface_set_margin(Resource *resource,
                                                             int32_t margin)
{
  qCDebug(shellExt) << Q_FUNC_INFO << margin;
  Q_UNUSED(resource)
  setMargin(margin);
}

void EmbeddedShellSurface::embedded_shell_surface_set_sort_index(
    Resource *resource, uint32_t sort_index)
{
  qCDebug(shellExt) << Q_FUNC_INFO << sort_index;
  Q_UNUSED(resource)
  setSortIndex(sort_index);
}

void EmbeddedShellSurface::embedded_shell_surface_set_app_id(
    Resource *resource, const QString &appId)
{
    qCDebug(shellExt) << Q_FUNC_INFO << appId;
    Q_UNUSED(resource)
    setAppId(appId);
}

void EmbeddedShellSurface::embedded_shell_surface_set_app_label(
    Resource *resource, const QString &appLabel)
{
    qCDebug(shellExt) << Q_FUNC_INFO << appLabel;
    Q_UNUSED(resource)
    setAppLabel(appLabel);
}

void EmbeddedShellSurface::embedded_shell_surface_set_app_icon(
    Resource *resource, const QString &appIcon)
{
    qCDebug(shellExt) << Q_FUNC_INFO << appIcon;
    Q_UNUSED(resource)
    setAppIcon(appIcon);
}

unsigned int EmbeddedShellSurfaceView::sortIndex() const
{
  return m_sortIndex;
}

void EmbeddedShellSurfaceView::setSortIndex(unsigned int newSortIndex)
{
  if (m_sortIndex == newSortIndex)
    return;
  m_sortIndex = newSortIndex;
  emit sortIndexChanged(m_sortIndex);
}

QVariant EmbeddedShellSurfaceView::customData() const
{
  return m_customData;
}

void EmbeddedShellSurfaceView::setCustomData(const QVariant &customData)
{
  if (m_customData == customData)
    return;
  m_customData = customData;
  emit customDataChanged(m_customData);
}

QString EmbeddedShellSurfaceView::uuid() const
{
  return m_uuid.toString(QUuid::WithoutBraces);
}

void EmbeddedShellSurfaceView::select()
{
  surface_view::send_selected();
}
