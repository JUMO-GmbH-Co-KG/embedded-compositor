// SPDX-License-Identifier: LGPL-3.0-only

#ifndef QUICKEMBEDDEDSHELLTYPES_H
#define QUICKEMBEDDEDSHELLTYPES_H

#include "embeddedplatform.h"
#include "embeddedshellsurface.h"
#include "embeddedshelltypes.h"

#include <QtQml>

namespace EmbeddedShellTypesForeign
{
  Q_NAMESPACE
  QML_NAMED_ELEMENT(EmbeddedShellTypes)
  QML_FOREIGN_NAMESPACE(EmbeddedShellTypes)
}

struct EmbeddedShellSurfaceViewForeign
{
  Q_GADGET
  QML_FOREIGN(EmbeddedShellSurfaceView)
  QML_NAMED_ELEMENT(SurfaceView)
  QML_UNCREATABLE("Created by wayland request only!")
};

struct EmbeddedPlatformForeign
{
  Q_GADGET
  QML_FOREIGN(EmbeddedPlatform)
  QML_NAMED_ELEMENT(Platform)
};

#endif // QUICKEMBEDDEDSHELLTYPES_H
