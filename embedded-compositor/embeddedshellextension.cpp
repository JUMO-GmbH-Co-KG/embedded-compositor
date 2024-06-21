// SPDX-License-Identifier: GPL-3.0-only

#include "embeddedshellextension.h"
#include "qwayland-server-embedded-shell.h"
#include <QWaylandResource>
#include <QWaylandSurface>
#include <QtWaylandCompositor/QWaylandResource>
#include <QtWaylandCompositor/private/qwaylandutils_p.h>

Q_LOGGING_CATEGORY(shellExt, "embeddedshell.compositor")

EmbeddedShellExtension::EmbeddedShellExtension(QWaylandCompositor *compositor)
    : QWaylandShellTemplate<EmbeddedShellExtension>(compositor) {
  qCDebug(shellExt) << __PRETTY_FUNCTION__ << compositor;
}

EmbeddedShellExtension::EmbeddedShellExtension()
    : QWaylandShellTemplate<EmbeddedShellExtension>() {
  qCDebug(shellExt) << __PRETTY_FUNCTION__;
}

void EmbeddedShellExtension::initialize() {
  qCDebug(shellExt) << __PRETTY_FUNCTION__;
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
    Resource *resource, wl_resource *wl_surface, uint32_t id, uint32_t anchor,
    uint32_t margin, unsigned int sort_index) {
  qCDebug(shellExt) << __PRETTY_FUNCTION__ << id << "anchor" << anchor
                    << "margin" << margin;
  Q_UNUSED(resource)

  QWaylandSurface *surface = QWaylandSurface::fromResource(wl_surface);

  QWaylandResource embeddedShellSurfaceResource(
      wl_resource_create(resource->client(), &embedded_shell_surface_interface,
                         wl_resource_get_version(resource->handle), id));
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
      m_surface(surface), m_anchor(anchor), m_margin(margin),
      m_sort_index(sort_index) {
  Q_UNUSED(ext)
  qCDebug(shellExt) << __PRETTY_FUNCTION__ << anchor
                    << wl_resource_get_id(resource.resource());
  init(resource.resource());
  setExtensionContainer(surface);
  QWaylandCompositorExtension::initialize();
}

QWaylandQuickShellIntegration *
EmbeddedShellSurface::createIntegration(QWaylandQuickShellSurfaceItem *item) {
  qCDebug(shellExt) << __PRETTY_FUNCTION__;
  return new QuickEmbeddedShellIntegration(item);
}

void EmbeddedShellSurface::setMargin(int newMargin) {
  qCDebug(shellExt) << __PRETTY_FUNCTION__ << newMargin;
  m_margin = newMargin;
  emit marginChanged(newMargin);
}

void EmbeddedShellSurface::setSortIndex(unsigned int sort_index) {
  m_sort_index = sort_index;
  emit sortIndexChanged(sort_index);
}

void EmbeddedShellSurface::sendConfigure(const QSize size) {
  qCDebug(shellExt) << __PRETTY_FUNCTION__ << size;
  send_configure(size.width(), size.height());
}

QString EmbeddedShellSurface::getUuid() const {
  return m_uuid.toString(QUuid::WithoutBraces);
}

pid_t EmbeddedShellSurface::getClientPid() const {
  pid_t pid;
  uid_t uid;
  gid_t gid;
  wl_client_get_credentials(resource()->client(), &pid, &uid, &gid);
  return pid;
}

QuickEmbeddedShellIntegration::QuickEmbeddedShellIntegration(
    QWaylandQuickShellSurfaceItem *item)
    : QWaylandQuickShellIntegration(item), m_item(item),
      m_shellSurface(
          qobject_cast<EmbeddedShellSurface *>(item->shellSurface())) {
  qCDebug(shellExt) << __PRETTY_FUNCTION__;
  m_item->setSurface(m_shellSurface->surface());
  connect(m_shellSurface, &EmbeddedShellSurface::destroyed, this,
          &QuickEmbeddedShellIntegration::handleEmbeddedShellSurfaceDestroyed);
}

QuickEmbeddedShellIntegration::~QuickEmbeddedShellIntegration() {
  qCDebug(shellExt) << __PRETTY_FUNCTION__;
  m_item->setSurface(nullptr);
}

void QuickEmbeddedShellIntegration::sendConfigure(const QSize size) {
  m_shellSurface->send_configure(size.width(), size.height());
}

void QuickEmbeddedShellIntegration::handleEmbeddedShellSurfaceDestroyed() {
  qCDebug(shellExt) << __PRETTY_FUNCTION__;
  m_shellSurface = nullptr;
}

void EmbeddedShellSurface::embedded_shell_surface_set_anchor(Resource *resource,
                                                             uint32_t anchor) {
  Q_UNUSED(resource)
  auto newAnchor = static_cast<EmbeddedShellTypes::Anchor>(anchor);
  qCDebug(shellExt) << __PRETTY_FUNCTION__ << m_anchor << "->" << newAnchor;
  if(newAnchor != m_anchor) {
    m_anchor = newAnchor;
    emit anchorChanged(m_anchor);
  }
}

void EmbeddedShellSurface::embedded_shell_surface_view_create(
    Resource *resource, wl_resource *shell_surface, const QString &appId, const QString &appLabel, const QString &appIcon,
    const QString &label, const QString &icon, uint32_t sort_index, uint32_t id) {
  Q_UNUSED(shell_surface)
  qCDebug(shellExt) << __PRETTY_FUNCTION__ << appId << appLabel << appIcon << label << icon << id;
  auto view = new EmbeddedShellSurfaceView(appId, appLabel, appIcon, label, icon, sort_index,
                                           resource->client(), id, 1);
  emit createView(view);
}

EmbeddedShellSurfaceView::EmbeddedShellSurfaceView(const QString &appId,
                                                   const QString &label,
                                                   const QString &icon,
                                                   uint32_t sort_index,
                                                   wl_client *client,
                                                   int id, int version)
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
                                                   wl_client *client,
                                                   int id, int version)
    : QtWaylandServer::surface_view(client, id, version)
    , m_appId(appId)
    , m_appLabel(appLabel)
    , m_appIcon(appIcon)
    , m_label(label)
    , m_icon(icon)
    , m_sortIndex(sort_index)
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
    Q_EMIT appIdChanged(appId);
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
    Q_EMIT appLabelChanged(appLabel);
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
    Q_EMIT appIconChanged(appIcon);
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
    Q_EMIT labelChanged(label);
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
    Q_EMIT iconChanged(icon);
}

void EmbeddedShellSurfaceView::surface_view_set_app_label(Resource *resource, const QString &label)
{
    qCDebug(shellExt) << __PRETTY_FUNCTION__ << label;
    Q_UNUSED(resource);
    setAppLabel(label);
}

void EmbeddedShellSurfaceView::surface_view_set_app_icon(Resource *resource, const QString &icon)
{
    qCDebug(shellExt) << __PRETTY_FUNCTION__ << icon;
    Q_UNUSED(resource);
    setAppIcon(icon);
}

void EmbeddedShellSurfaceView::surface_view_set_label(Resource *resource, const QString &text)
{
    qCDebug(shellExt) << __PRETTY_FUNCTION__ << text;
    Q_UNUSED(resource)
    setLabel(text);
}

void EmbeddedShellSurfaceView::surface_view_set_icon(Resource *resource, const QString &icon)
{
    qCDebug(shellExt) << __PRETTY_FUNCTION__ << icon;
    Q_UNUSED(resource)
    setIcon(icon);
}

void EmbeddedShellSurface::embedded_shell_surface_set_margin(Resource *resource,
                                                             int32_t margin) {
  qCDebug(shellExt) << __PRETTY_FUNCTION__ << margin;
  Q_UNUSED(resource)
  setMargin(margin);
}

void EmbeddedShellSurface::embedded_shell_surface_set_sort_index(
    Resource *resource, uint32_t sort_index) {
  qCDebug(shellExt) << __PRETTY_FUNCTION__ << sort_index;
  Q_UNUSED(resource)
  setSortIndex(sort_index);
}

unsigned int EmbeddedShellSurfaceView::sortIndex() const { return m_sortIndex; }

void EmbeddedShellSurfaceView::setSortIndex(unsigned int newSortIndex) {
  if (m_sortIndex == newSortIndex)
    return;
  m_sortIndex = newSortIndex;
  emit sortIndexChanged(m_sortIndex);
}

QString EmbeddedShellSurfaceView::getUuid() const {
  return m_uuid.toString(QUuid::WithoutBraces);
}

void EmbeddedShellSurfaceView::surface_view_set_sort_index(Resource *resource,
                                                           uint32_t sort_index) {
  Q_UNUSED(resource)
  setSortIndex(sort_index);
}
