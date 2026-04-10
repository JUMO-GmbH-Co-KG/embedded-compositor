// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "embeddedshellsurfaceview.h"
#include "qwayland-embedded-shell.h"

#include <QVariant>

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

  static QByteArray serializeVariantMap(const QVariantMap &variantMap);

  EmbeddedShellSurfaceView *q_ptr = nullptr;

  EmbeddedShellSurfaceView *m_parentView = nullptr;
  QString m_label;
  QString m_icon;
  uint32_t m_sortIndex = 0;
  QVariantMap m_customData;
  bool m_selected;
  bool m_topLevel;
};
