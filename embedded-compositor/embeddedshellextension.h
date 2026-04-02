// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <QtWaylandCompositor/QWaylandShellSurfaceTemplate>
#include <QtWaylandCompositor/QWaylandShellTemplate>

#include "quuid.h"
#include "qwayland-server-embedded-shell.h"
#include <QWaylandResource>
#include <QtWaylandCompositor/QWaylandCompositor>
#include <QtWaylandCompositor/QWaylandQuickExtension>

#include <QtWaylandCompositor/QWaylandQuickShellIntegration>
#include <QtWaylandCompositor/QWaylandQuickShellSurfaceItem>

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
  void embedded_shell_surface_create(Resource *resource,
                                     struct ::wl_resource *surface, uint32_t id,
                                     uint32_t anchor, uint32_t margin,
                                     uint sort_index) override;

  void initialize() override;
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
  Q_PROPERTY(unsigned int sortIndex READ sortIndex NOTIFY sortIndexChanged)
  Q_PROPERTY(QString appId READ appId CONSTANT)
  Q_PROPERTY(QString appLabel READ appLabel NOTIFY appLabelChanged)
  Q_PROPERTY(QString appIcon READ appIcon NOTIFY appIconChanged)
  Q_PROPERTY(QSize size READ size NOTIFY sizeChanged)
  Q_PROPERTY(QVariant customData READ customData NOTIFY customDataChanged)
  Q_PROPERTY(QString uuid READ uuid CONSTANT)

public:
  EmbeddedShellSurface(EmbeddedShellExtension *ext, QWaylandSurface *surface,
                       const QWaylandResource &resource,
                       EmbeddedShellTypes::Anchor anchor, uint32_t margin,
                       uint32_t sort_index);
  QWaylandQuickShellIntegration *
  createIntegration(QWaylandQuickShellSurfaceItem *item) override;

  QWaylandSurface *surface() const;

  EmbeddedShellTypes::Anchor anchor();
  void setAnchor(embedded_shell_anchor_border newAnchor);

  int margin();
  void setMargin(int newMargin);

  unsigned int sortIndex();
  void setSortIndex(unsigned int sort_index);

  QString appId() const;
  void setAppId(const QString &appId);

  QString appLabel() const;
  void setAppLabel(const QString &appLabel);

  QString appIcon() const;
  void setAppIcon(const QString &appIcon);

  QSize size() const;
  void setSize(const QSize &size);

  QVariant customData() const;
  void setCustomData(const QVariant &customData);

  QString uuid() const;

  Q_INVOKABLE void sendConfigure(const QSize size);
  Q_INVOKABLE void sendVisibleChanged(bool visible);
  pid_t getClientPid() const;

signals:
  void anchorChanged(EmbeddedShellTypes::Anchor anchor);
  void marginChanged(int margin);
  void sortIndexChanged(unsigned int sort_index);
  void appIconChanged(const QString &appIcon);
  void appLabelChanged(const QString &appLabel);
  void sizeChanged(const QSize &size);
  void customDataChanged(const QVariant &customData);
  void createView(EmbeddedShellSurfaceView *view);

private:
  QWaylandSurface *m_surface;
  EmbeddedShellTypes::Anchor m_anchor = EmbeddedShellTypes::Anchor::Undefined;
  uint32_t m_margin = 0;
  uint32_t m_sort_index = 0;
  QString m_appId;
  QString m_appLabel;
  QString m_appIcon;
  QSize m_size;
  QVariant m_customData;
  QUuid m_uuid = QUuid::createUuid();

  // embedded_shell_surface interface
protected:
  void embedded_shell_surface_set_anchor(Resource *resource,
                                         uint32_t anchor) override;
  void embedded_shell_surface_set_margin(Resource *resource,
                                         int32_t margin) override;
  void embedded_shell_surface_set_sort_index(Resource *resource,
                                             uint32_t sort_index) override;
  void embedded_shell_surface_set_app_id(Resource *resource,
                                         const QString &appId) override;
  void embedded_shell_surface_set_app_label(Resource *resource,
                                            const QString &label) override;
  void embedded_shell_surface_set_app_icon(Resource *resource,
                                           const QString &icon) override;
  void embedded_shell_surface_set_size(Resource *resource,
                                       uint32_t width,
                                       uint32_t height) override;
  void embedded_shell_surface_set_custom_data(Resource *resource,
                                              wl_array *custom_data) override;
  void embedded_shell_surface_view_create(Resource *resource,
                                          wl_resource *shell_surface,
                                          const QString &appId,
                                          const QString &appLabel,
                                          const QString &appIcon,
                                          const QString &label,
                                          const QString &icon,
                                          unsigned int sort_index,
                                          wl_array *custom_data,
                                          uint32_t id) override;
};

class EmbeddedShellSurfaceView : public QObject,
                                 public QtWaylandServer::surface_view {
  Q_OBJECT
  Q_PROPERTY(QString appId READ appId WRITE setAppId NOTIFY appIdChanged)
  Q_PROPERTY(QString appLabel READ appLabel WRITE setAppLabel NOTIFY appLabelChanged)
  Q_PROPERTY(QString appIcon READ appIcon WRITE setAppIcon NOTIFY appIconChanged)
  Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
  Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
  Q_PROPERTY(unsigned int sortIndex READ sortIndex NOTIFY sortIndexChanged)
  Q_PROPERTY(QVariant customData READ customData NOTIFY customDataChanged)
  Q_PROPERTY(QString uuid READ uuid CONSTANT)

public:
  EmbeddedShellSurfaceView(const QString &appId,
                           const QString &label,
                           const QString &icon,
                           uint32_t sort_index,
                           wl_client *client,
                           int id,
                           int version);
  EmbeddedShellSurfaceView(const QString &appId,
                           const QString &appLabel,
                           const QString &appIcon,
                           const QString &label,
                           const QString &icon,
                           uint32_t sort_index,
                           const QVariant &custom_data,
                           wl_client *client,
                           int id,
                           int version);

  QString appId() const;
  void setAppId(const QString &appId);

  QString appLabel() const;
  void setAppLabel(const QString &appLabel);

  QString appIcon() const;
  void setAppIcon(const QString &appIcon);

  QString label() const;
  void setLabel(const QString &label);

  QString icon() const;
  void setIcon(const QString &icon);

  unsigned int sortIndex() const;
  void setSortIndex(unsigned int newSortIndex);

  QVariant customData() const;
  void setCustomData(const QVariant &customData);

  QString uuid() const;

  Q_INVOKABLE void select();

signals:
  void appIdChanged(const QString &appId);
  void appLabelChanged(const QString &appLabel);
  void appIconChanged(const QString &appIcon);
  void labelChanged(const QString &label);
  void iconChanged(const QString &icon);
  void sortIndexChanged(unsigned int index);
  void customDataChanged(const QVariant &customData);
  void aboutToBeSelected();
  void aboutToBeDestroyed();

protected:
  void surface_view_set_app_label(Resource *resource,
                                  const QString &label) override;
  void surface_view_set_app_icon(Resource *resource,
                                 const QString &icon) override;
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
  QUuid m_uuid = QUuid::createUuid();
  QString m_appId;
  QString m_appLabel;
  QString m_appIcon;
  QString m_label;
  QString m_icon;
  uint32_t m_sortIndex = 0;
  QVariant m_customData;
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
