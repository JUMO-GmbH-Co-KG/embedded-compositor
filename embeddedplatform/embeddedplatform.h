// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "embeddedshellsurface.h"

#include <QWindow>
#include <QObject>

class EmbeddedPlatform : public QObject
{
  Q_OBJECT

public:
  EmbeddedPlatform();

  static EmbeddedShellSurface *shellSurfaceForWindow(QWindow *window);

  static EmbeddedPlatform *s_instance;

  static EmbeddedPlatform *instance();

signals:
  void shellSurfaceCreated(EmbeddedShellSurface *, QWindow *);
};
