// SPDX-License-Identifier: LGPL-3.0-only

#ifndef EMBEDDEDSHELLSURFACEVIEW_P_H
#define EMBEDDEDSHELLSURFACEVIEW_P_H

#include "embeddedshellsurfaceview.h"
#include "qwayland-embedded-shell.h"

class EmbeddedShellSurfaceViewPrivate : public QObject,
                                        public QtWayland::surface_view
{
  Q_DECLARE_PUBLIC(EmbeddedShellSurfaceView)
  Q_OBJECT

public:
  EmbeddedShellSurfaceViewPrivate(EmbeddedShellSurfaceView *q,
                                  ::surface_view *view,
                                  EmbeddedShellSurface *surf);

  ~EmbeddedShellSurfaceViewPrivate() override;

  static EmbeddedShellSurfaceViewPrivate *get(EmbeddedShellSurfaceView *q);

  void surface_view_selected() override;

  EmbeddedShellSurfaceView *q_ptr = nullptr;

  QString m_appId;
  QString m_appLabel;
  QString m_appIcon;
  QString m_label;
  QString m_icon;
  uint32_t m_sortIndex;
  bool m_selected;
};
#endif // EMBEDDEDSHELLSURFACEVIEW_P_H
