#include "quickembeddedshell.h"
#include "quickembeddedshellwindow.h"
#include "embeddedshellsurface.h"
#include "waylandinputregion.h"

void QuickEmbeddedShell::registerTypes(const char *uri) {
  qmlRegisterType<QuickEmbeddedShellWindow>(uri, 1, 0, "Window");
  qmlRegisterType<WaylandInputRegion>(uri, 1, 0, "WaylandInputRegion");
  qmlRegisterType<EmbeddedPlatform>(uri, 1, 0, "Platform");
  qmlRegisterUncreatableType<EmbeddedShellSurfaceView>(
      uri, 1, 0, "SurfaceView", "created by wayland request only");
  qmlRegisterUncreatableMetaObject(EmbeddedShellTypes::staticMetaObject, uri, 1,
                                   0, "EmbeddedShellTypes",
                                   "uncreatable enums namespace");
}
