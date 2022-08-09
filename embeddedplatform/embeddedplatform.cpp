#include "embeddedplatform.h"
#include <QDebug>
#include <QGuiApplication>
#include <QWindow>
#include <qpa/qplatformnativeinterface.h>
EmbeddedPlatform::EmbeddedPlatform() { qDebug() << __PRETTY_FUNCTION__; }

EmbeddedPlatform::~EmbeddedPlatform() {}

EmbeddedShellSurface *EmbeddedPlatform::shellSurfaceForWindow(QWindow *window) {
  if(window == nullptr) return nullptr;
  auto pni = QGuiApplication::platformNativeInterface();
  return static_cast<EmbeddedShellSurface *>(
      pni->nativeResourceForWindow("embedded-shell-surface", window));
}
