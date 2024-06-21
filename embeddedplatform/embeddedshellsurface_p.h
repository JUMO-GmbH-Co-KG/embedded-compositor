// SPDX-License-Identifier: LGPL-3.0-only

#ifndef EMBEDDEDSHELLSURFACE_P_H
#define EMBEDDEDSHELLSURFACE_P_H

#include "embeddedshellsurface.h"
#include "qwayland-embedded-shell.h"
#include <QDebug>
#include <QtWaylandClient/private/qwaylandshellsurface_p.h>

class EmbeddedShellSurfacePrivate
    : public QtWaylandClient::QWaylandShellSurface,
      public QtWayland::embedded_shell_surface {
  Q_DECLARE_PUBLIC(EmbeddedShellSurface)
  Q_OBJECT
public:
  EmbeddedShellSurfacePrivate(struct ::embedded_shell_surface *shell_surface,
                              QtWaylandClient::QWaylandWindow *window,
                              EmbeddedShellTypes::Anchor anchor,
                              uint32_t margin, uint32_t sort_index);
  ~EmbeddedShellSurfacePrivate() override;
  EmbeddedShellTypes::Anchor getAnchor() const { return m_anchor; }
  uint32_t getMargin() const { return m_margin; }

  void applyConfigure() override;

private:
  EmbeddedShellTypes::Anchor m_anchor;
  uint32_t m_margin = 0;
  uint32_t m_sort_index = 0;
  QSize m_pendingSize = {0, 0};
  EmbeddedShellSurface *q_ptr = nullptr;

protected:
  void embedded_shell_surface_configure(int32_t width, int32_t height) override;
};

class EmbeddedShellSurfaceViewPrivate : public QObject,
                                        public QtWayland::surface_view {
  Q_DECLARE_PUBLIC(EmbeddedShellSurfaceView)
  Q_OBJECT
  QString m_label;

public:
  EmbeddedShellSurfaceViewPrivate(EmbeddedShellSurfaceView *q,
                                  ::surface_view *view,
                                  EmbeddedShellSurface *surf,
                                  const QString &label);
  void surface_view_selected() override {
    qDebug() << __PRETTY_FUNCTION__;
    Q_Q(EmbeddedShellSurfaceView);
    emit q->selected();
  }
  EmbeddedShellSurfaceView *q_ptr = nullptr;
};
#endif // EMBEDDEDSHELLSURFACE_P_H
