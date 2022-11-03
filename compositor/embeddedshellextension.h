#ifndef EMBEDDEDSHELLEXTENSION_H
#define EMBEDDEDSHELLEXTENSION_H

#include <QtWaylandCompositor/QWaylandShellSurfaceTemplate>
#include <QtWaylandCompositor/QWaylandShellTemplate>

#include "qwayland-server-embedded-shell.h"
#include <QWaylandResource>
#include <QtWaylandCompositor/QWaylandCompositor>
#include <QtWaylandCompositor/QWaylandQuickExtension>

#include <QtWaylandCompositor/QWaylandQuickShellIntegration>
#include <QtWaylandCompositor/QWaylandQuickShellSurfaceItem>

class EmbeddedShellSurface;
class EmbeddedShellSurfaceView;

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

  void setAnchor(embedded_shell_anchor_border newAnchor);
  void setMargin(int newMargin);
  void setSortIndex(int sort_index);
  Q_INVOKABLE void sendConfigure(const QSize size);

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

  // embedded_shell_surface interface
protected:
  void embedded_shell_surface_set_anchor(Resource *resource,
                                         uint32_t anchor) override;
  void embedded_shell_surface_view_create(Resource *resource,
                                          wl_resource *shell_surface,
                                          const QString &label, int sort_index,
                                          uint32_t id) override;
  void embedded_shell_surface_set_margin(Resource *resource,
                                         int32_t margin) override;
  void embedded_shell_surface_set_sort_index(Resource *resource,
                                             int32_t sort_index) override;
};

class EmbeddedShellSurfaceView : public QObject,
                                 public QtWaylandServer::surface_view {
  Q_OBJECT
  Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
  Q_PROPERTY(int sortIndex READ sortIndex NOTIFY sortIndexChanged)
public:
  EmbeddedShellSurfaceView(const QString &label, int32_t sort_index,
                           wl_client *client, int id, int version)
      : QtWaylandServer::surface_view(client, id, version), m_label(label),
        m_sortIndex(sort_index) {}
  const QString &label() const { return m_label; }
  void setLabel(const QString &newLabel);

  int sortIndex() const;
  void setSortIndex(int newSortIndex);

public slots:
  void select() { surface_view::send_selected(); }
signals:
  void labelChanged();
  void sortIndexChanged(int index);

protected:
  void surface_view_set_label(Resource *resource, const QString &text) override;
  void surface_view_set_sort_index(Resource *resource,
                                   int32_t sort_index) override;

private:
  QString m_label;
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
