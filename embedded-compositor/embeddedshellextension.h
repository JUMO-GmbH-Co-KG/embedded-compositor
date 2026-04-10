// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "qwayland-server-embedded-shell.h"

#include <QtWaylandCompositor/QWaylandShellSurfaceTemplate>
#include <QtWaylandCompositor/QWaylandShellTemplate>
#include <QtWaylandCompositor/QWaylandCompositor>
#include <QtWaylandCompositor/QWaylandQuickExtension>
#include <QtWaylandCompositor/QWaylandQuickShellIntegration>
#include <QtWaylandCompositor/QWaylandQuickShellSurfaceItem>
#include <QWaylandResource>

#include <QUuid>

class EmbeddedShellSurface;
class EmbeddedShellSurfaceView;

Q_DECLARE_LOGGING_CATEGORY(shellExt)

namespace EmbeddedShellTypes {
Q_NAMESPACE
#include "embeddedshellanchor.h"
Q_ENUM_NS(Anchor)
} // namespace EmbeddedShellTypes

class EmbeddedShellExtension
    : public QWaylandShellTemplate<EmbeddedShellExtension>,
      public QtWaylandServer::embedded_shell
{
  Q_OBJECT
public:
  explicit EmbeddedShellExtension(QWaylandCompositor *compositor);
  explicit EmbeddedShellExtension();
  void initialize() override;
  void embedded_shell_surface_create(Resource *resource,
                                     struct ::wl_resource *surface,
                                     uint32_t id,
                                     uint32_t anchor,
                                     uint32_t margin) override;

signals:
  void surfaceAdded(EmbeddedShellSurface *surface);
};

class EmbeddedShellSurface
    : public QWaylandShellSurfaceTemplate<EmbeddedShellSurface>,
      public QtWaylandServer::embedded_shell_surface
{
  Q_OBJECT
  Q_PROPERTY(EmbeddedShellTypes::Anchor anchor READ anchor NOTIFY anchorChanged)
  Q_PROPERTY(int margin READ margin NOTIFY marginChanged)
  Q_PROPERTY(QSize size READ size NOTIFY sizeChanged)
  Q_PROPERTY(QString uuid READ uuid CONSTANT)

public:
  EmbeddedShellSurface(EmbeddedShellExtension *extension,
                       QWaylandSurface *surface,
                       const QWaylandResource &resource,
                       EmbeddedShellTypes::Anchor anchor,
                       uint32_t margin);

  QWaylandQuickShellIntegration *createIntegration(QWaylandQuickShellSurfaceItem *item) override;

  QWaylandSurface *surface() const;

  EmbeddedShellTypes::Anchor anchor();
  int margin();
  QSize size() const;
  QString uuid() const;

  Q_INVOKABLE void sendConfigure(const QSize size);
  Q_INVOKABLE void sendVisibleChanged(bool visible);
  pid_t getClientPid() const;

signals:
  void anchorChanged(EmbeddedShellTypes::Anchor anchor);
  void marginChanged(int margin);
  void sizeChanged(const QSize &size);
  void createView(EmbeddedShellSurfaceView *view);

private:
  void updateAnchor(EmbeddedShellTypes::Anchor newAnchor);
  void updateMargin(int newMargin);
  void updateSize(const QSize &size);

private:
  QWaylandSurface *m_surface;
  EmbeddedShellTypes::Anchor m_anchor = EmbeddedShellTypes::Anchor::Undefined;
  uint32_t m_margin = 0;
  QSize m_size;
  QUuid m_uuid = QUuid::createUuid();

  // embedded_shell_surface interface
protected:
  void embedded_shell_surface_set_anchor(Resource *resource,
                                         uint32_t anchor) override;
  void embedded_shell_surface_set_margin(Resource *resource,
                                         int32_t margin) override;
  void embedded_shell_surface_set_size(Resource *resource,
                                       uint32_t width,
                                       uint32_t height) override;
  void embedded_shell_surface_view_create(Resource *resource,
                                          wl_resource *shell_surface,
                                          const QString &label,
                                          const QString &icon,
                                          unsigned int sort_index,
                                          const QString &persistentId,
                                          wl_array *custom_data,
                                          wl_resource *parent_view,
                                          uint32_t id) override;
};

class EmbeddedShellSurfaceView : public QObject,
                                 public QtWaylandServer::surface_view {
  Q_OBJECT
  Q_PROPERTY(QString label READ label NOTIFY labelChanged)
  Q_PROPERTY(QString icon READ icon NOTIFY iconChanged)
  Q_PROPERTY(unsigned int sortIndex READ sortIndex NOTIFY sortIndexChanged)
  Q_PROPERTY(QVariantMap customData READ customData NOTIFY customDataChanged)
  Q_PROPERTY(QString parentUuid READ parentUuid CONSTANT)
  Q_PROPERTY(QString uuid READ uuid CONSTANT)
  Q_PROPERTY(bool isPersistent READ isPersistent CONSTANT)

public:
  EmbeddedShellSurfaceView(const QString &label,
                           const QString &icon,
                           uint32_t sortIndex,
                           const QString &persistentId,
                           const QVariantMap &customData,
                           wl_client *client,
                           EmbeddedShellSurfaceView *parentView,
                           int id,
                           int version);

  QString label() const;
  QString icon() const;
  unsigned int sortIndex() const;
  QVariantMap customData() const;
  QString parentUuid() const;
  QString uuid() const;
  bool isPersistent() const;

  Q_INVOKABLE void select();

signals:
  void labelChanged(const QString &label);
  void iconChanged(const QString &icon);
  void sortIndexChanged(unsigned int index);
  void customDataChanged(const QVariantMap &customData);
  void aboutToBeSelected();
  void aboutToBeDestroyed();

private:
  void updateLabel(const QString &label);
  void updateIcon(const QString &icon);
  void updateSortIndex(unsigned int newSortIndex);
  void updateCustomData(const QVariantMap &customData);

protected:
  void surface_view_set_label(Resource *resource,
                              const QString &text) override;
  void surface_view_set_icon(Resource *resource,
                             const QString &icon) override;
  void surface_view_set_sort_index(Resource *resource,
                                   uint32_t sort_index) override;
  void surface_view_set_custom_data(Resource *resource,
                                    wl_array *custom_data) override;
  void surface_view_select(Resource *resource) override;
  void surface_view_destroy(Resource *resource) override;

private:
  bool m_isPersistent = false;
  QString m_uuid;
  QString m_label;
  QString m_icon;
  uint32_t m_sortIndex = 0;
  QVariantMap m_customData;
  EmbeddedShellSurfaceView * m_parentView = nullptr;
};

class QuickEmbeddedShellIntegration : public QWaylandQuickShellIntegration
{
  Q_OBJECT
public:
  QuickEmbeddedShellIntegration(QWaylandQuickShellSurfaceItem *item);
  ~QuickEmbeddedShellIntegration() override;

  Q_PROPERTY(EmbeddedShellTypes::Anchor anchor READ getAnchor)
  Q_PROPERTY(int margin READ getMargin)

  EmbeddedShellTypes::Anchor getAnchor();
  int getMargin();
  void sendConfigure(const QSize size);

private:
  void handleEmbeddedShellSurfaceDestroyed();

private:
  QWaylandQuickShellSurfaceItem *m_item = nullptr;
  EmbeddedShellSurface *m_shellSurface = nullptr;
};

Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(EmbeddedShellExtension)
