// SPDX-License-Identifier: LGPL-3.0-only

#ifndef EMBEDDEDSHELLSURFACE_P_H
#define EMBEDDEDSHELLSURFACE_P_H

#include "embeddedshellsurface.h"
#include "qwayland-embedded-shell.h"

#include <QPointer>
#include <QtWaylandClient/private/qwaylandshellsurface_p.h>

class EmbeddedShellSurfacePrivate : public QtWaylandClient::QWaylandShellSurface,
                                    public QtWayland::embedded_shell_surface
{
  Q_DECLARE_PUBLIC(EmbeddedShellSurface)
  Q_OBJECT

public:
  EmbeddedShellSurfacePrivate(EmbeddedShellSurface *q,
                              struct ::embedded_shell_surface *shell_surface,
                              QtWaylandClient::QWaylandWindow *window,
                              const QSize &size,
                              EmbeddedShellTypes::Anchor anchor,
                              uint32_t margin, uint32_t sort_index);

  ~EmbeddedShellSurfacePrivate() override;

  QSize getSize() const { return m_size; }
  EmbeddedShellTypes::Anchor getAnchor() const { return m_anchor; }
  uint32_t getMargin() const { return m_margin; }

  void applyConfigure() override;

private:
  QSize m_size;
  EmbeddedShellTypes::Anchor m_anchor;
  uint32_t m_margin = 0;
  uint32_t m_sort_index = 0;
  bool m_visible = false;
  QSize m_pendingSize = {0, 0};
  QPointer<EmbeddedShellSurfaceView> m_selectedView;
  EmbeddedShellSurface *q_ptr = nullptr;

protected:
  void embedded_shell_surface_configure(int32_t width, int32_t height) override;
  void embedded_shell_surface_visible_changed(int32_t visible) override;
};

#endif // EMBEDDEDSHELLSURFACE_P_H
