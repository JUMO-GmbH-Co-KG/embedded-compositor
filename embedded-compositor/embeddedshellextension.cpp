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
    uint32_t margin, int sort_index) {
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
                                           uint32_t margin, int32_t sort_index)
    : QWaylandShellSurfaceTemplate<EmbeddedShellSurface>(this),
      m_surface(surface), m_anchor(anchor), m_margin(margin),
      m_sort_index(sort_index) {
  Q_UNUSED(ext)
  qCDebug(shellExt) << __PRETTY_FUNCTION__ << anchor;
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

void EmbeddedShellSurface::setSortIndex(int sort_index) {
  m_sort_index = sort_index;
  emit sortIndexChanged(sort_index);
}

void EmbeddedShellSurface::sendConfigure(const QSize size) {
  qCDebug(shellExt) << __PRETTY_FUNCTION__ << size;
  send_configure(size.width(), size.height());
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
  qCDebug(shellExt) << __PRETTY_FUNCTION__ << m_anchor << "->" << anchor;
  m_anchor = static_cast<EmbeddedShellTypes::Anchor>(anchor);
  qCDebug(shellExt) << "emitting" << m_anchor;
  emit anchorChanged(m_anchor);
}

void EmbeddedShellSurface::embedded_shell_surface_view_create(
    Resource *resource, wl_resource *shell_surface, const QString &label,
    int32_t sort_index, uint32_t id) {
  Q_UNUSED(shell_surface)
  qCDebug(shellExt) << __PRETTY_FUNCTION__ << label << id;
  auto view = new EmbeddedShellSurfaceView(label, sort_index,
                                           resource->client(), id, 1);
  emit createView(view);
}

void EmbeddedShellSurfaceView::setLabel(const QString &newLabel) {
  if (m_label == newLabel)
    return;
  m_label = newLabel;
  emit labelChanged();
}

void EmbeddedShellSurfaceView::surface_view_set_label(Resource *resource,
                                                      const QString &text) {
  qCDebug(shellExt) << __PRETTY_FUNCTION__ << text;
  Q_UNUSED(resource)
  setLabel(text);
}

void EmbeddedShellSurface::embedded_shell_surface_set_margin(Resource *resource,
                                                             int32_t margin) {
  qCDebug(shellExt) << __PRETTY_FUNCTION__ << margin;
  Q_UNUSED(resource)
  setMargin(margin);
}

void EmbeddedShellSurface::embedded_shell_surface_set_sort_index(
    Resource *resource, int32_t sort_index) {
  qCDebug(shellExt) << __PRETTY_FUNCTION__ << sort_index;
  Q_UNUSED(resource)
  setSortIndex(sort_index);
}

int EmbeddedShellSurfaceView::sortIndex() const { return m_sortIndex; }

void EmbeddedShellSurfaceView::setSortIndex(int newSortIndex) {
  if (m_sortIndex == newSortIndex)
    return;
  m_sortIndex = newSortIndex;
  emit sortIndexChanged(m_sortIndex);
}

void EmbeddedShellSurfaceView::surface_view_set_sort_index(Resource *resource,
                                                           int32_t sort_index) {
  Q_UNUSED(resource)
  setSortIndex(sort_index);
}
