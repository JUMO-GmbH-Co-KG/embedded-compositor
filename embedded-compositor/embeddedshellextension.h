// SPDX-License-Identifier: GPL-3.0-only

#ifndef EMBEDDEDSHELLEXTENSION_H
#define EMBEDDEDSHELLEXTENSION_H

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
      public QtWaylandServer::embedded_shell {
  Q_OBJECT
public:
  explicit EmbeddedShellExtension(QWaylandCompositor *compositor);
  explicit EmbeddedShellExtension();
  void embedded_shell_surface_create(Resource *resource,
                                     struct ::wl_resource *surface, uint32_t id,
                                     uint32_t anchor, uint32_t margin,
                                     int sort_index) override;

  void initialize() override;
signals:
  void surfaceAdded(EmbeddedShellSurface *surface);
};

class EmbeddedShellSurface
    : public QWaylandShellSurfaceTemplate<EmbeddedShellSurface>,
      public QtWaylandServer::embedded_shell_surface {
  Q_OBJECT
public:
  EmbeddedShellSurface(EmbeddedShellExtension *ext, QWaylandSurface *surface,
                       const QWaylandResource &resource,
                       EmbeddedShellTypes::Anchor anchor, uint32_t margin,
                       int32_t sort_index);
  QWaylandQuickShellIntegration *
  createIntegration(QWaylandQuickShellSurfaceItem *item) override;

  QWaylandSurface *surface() const { return m_surface; }

  EmbeddedShellTypes::Anchor getAnchor() { return m_anchor; }
  int getMargin() { return m_margin; }
  int sortIndex() { return m_sort_index; }
  Q_PROPERTY(
      EmbeddedShellTypes::Anchor anchor READ getAnchor NOTIFY anchorChanged)
  Q_PROPERTY(int margin READ getMargin NOTIFY marginChanged)
  Q_PROPERTY(int sortIndex READ sortIndex NOTIFY sortIndexChanged)
  Q_PROPERTY(QString uuid READ getUuid CONSTANT)

  void setAnchor(embedded_shell_anchor_border newAnchor);
  void setMargin(int newMargin);
  void setSortIndex(int sort_index);
  Q_INVOKABLE void sendConfigure(const QSize size);
  QString getUuid() const;
  pid_t getClientPid() const;

signals:
  void anchorChanged(EmbeddedShellTypes::Anchor anchor);
  void marginChanged(int margin);
  void sortIndexChanged(int sort_index);
  void createView(EmbeddedShellSurfaceView *view);

private:
  QWaylandSurface *m_surface;
  EmbeddedShellTypes::Anchor m_anchor = EmbeddedShellTypes::Anchor::Undefined;
  uint32_t m_margin = 0;
  int32_t m_sort_index = 0;
  QUuid m_uuid = QUuid::createUuid();

  // embedded_shell_surface interface
protected:
  void embedded_shell_surface_set_anchor(Resource *resource,
                                         uint32_t anchor) override;
  void embedded_shell_surface_view_create(Resource *resource,
                                          wl_resource *shell_surface,
                                          const QString &appId, const QString &appLabel, const QString &appIcon,
                                          const QString &label, const QString &icon, int sort_index,
                                          uint32_t id) override;
  void embedded_shell_surface_set_margin(Resource *resource,
                                         int32_t margin) override;
  void embedded_shell_surface_set_sort_index(Resource *resource,
                                             int32_t sort_index) override;
};

class EmbeddedShellSurfaceView : public QObject,
                                 public QtWaylandServer::surface_view {
  Q_OBJECT
  Q_PROPERTY(QString appId READ appId WRITE setAppId NOTIFY appIdChanged)
  Q_PROPERTY(QString appLabel READ appLabel WRITE setAppLabel NOTIFY appLabelChanged)
  Q_PROPERTY(QString appIcon READ appIcon WRITE setAppIcon NOTIFY appIconChanged)
  Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
  Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
  Q_PROPERTY(int sortIndex READ sortIndex NOTIFY sortIndexChanged)
  Q_PROPERTY(QString uuid READ getUuid CONSTANT)
public:
  EmbeddedShellSurfaceView(const QString &appId, const QString &label, const QString &icon,
                           int32_t sort_index, wl_client *client, int id, int version);
  EmbeddedShellSurfaceView(const QString &appId, const QString &appLabel, const QString &appIcon,
                           const QString &label, const QString &icon, int32_t sort_index,
                           wl_client *client, int id, int version);

  QString appId() const;
  void setAppId(const QString &appId);
  Q_SIGNAL void appIdChanged(const QString &appId);

  QString appLabel() const;
  void setAppLabel(const QString &appLabel);
  Q_SIGNAL void appLabelChanged(const QString &appLabel);

  QString appIcon() const;
  void setAppIcon(const QString &appIcon);
  Q_SIGNAL void appIconChanged(const QString &appIcon);

  QString label() const;
  void setLabel(const QString &label);
  Q_SIGNAL void labelChanged(const QString &label);

  QString icon() const;
  void setIcon(const QString &icon);
  Q_SIGNAL void iconChanged(const QString &icon);

  int sortIndex() const;
  void setSortIndex(int newSortIndex);

  QString getUuid() const;

public slots:
  void select() { surface_view::send_selected(); }
signals:
  void sortIndexChanged(int index);

protected:
  void surface_view_set_app_label(Resource *resource, const QString &label) override;
  void surface_view_set_app_icon(Resource *resource, const QString &icon) override;
  void surface_view_set_label(Resource *resource, const QString &text) override;
  void surface_view_set_icon(Resource *resource, const QString &icon) override;
  void surface_view_set_sort_index(Resource *resource,
                                   int32_t sort_index) override;

private:
  QUuid m_uuid = QUuid::createUuid();
  QString m_appId;
  QString m_appLabel;
  QString m_appIcon;
  QString m_label;
  QString m_icon;
  int32_t m_sortIndex = 0;
};

class QuickEmbeddedShellIntegration : public QWaylandQuickShellIntegration {
  Q_OBJECT
public:
  QuickEmbeddedShellIntegration(QWaylandQuickShellSurfaceItem *item);
  ~QuickEmbeddedShellIntegration() override;

  Q_PROPERTY(EmbeddedShellTypes::Anchor anchor READ getAnchor)
  Q_PROPERTY(int margin READ getMargin)

  EmbeddedShellTypes::Anchor getAnchor() { return m_shellSurface->getAnchor(); }
  int getMargin() { return m_shellSurface->getMargin(); }
  void sendConfigure(const QSize size);

private slots:
  void handleEmbeddedShellSurfaceDestroyed();

private:
  QWaylandQuickShellSurfaceItem *m_item = nullptr;
  EmbeddedShellSurface *m_shellSurface = nullptr;
};

Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(EmbeddedShellExtension)

#endif // EMBEDDEDSHELLEXTENSION_H
