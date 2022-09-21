#ifndef EMBEDDEDPLATFORM_H
#define EMBEDDEDPLATFORM_H
#include <QObject>

class EmbeddedShellSurface;
class QWindow;

namespace EmbeddedShellTypes {
Q_NAMESPACE_EXPORT(Q_DECL_EXPORT)
#include "embeddedshellanchor.h"
Q_ENUM_NS(Anchor)
} // namespace EmbeddedShellTypes

class EmbeddedPlatform : public QObject {
  Q_OBJECT
public:
  EmbeddedPlatform();
  ~EmbeddedPlatform() override;

  static EmbeddedShellSurface *shellSurfaceForWindow(QWindow *window);

  static EmbeddedPlatform *s_instance;

  static EmbeddedPlatform *instance() {
    if (s_instance == nullptr) {
      s_instance = new EmbeddedPlatform();
    }
    return s_instance;
  }

signals:
  void shellSurfaceCreated(EmbeddedShellSurface *, QWindow *);
};

#endif // EMBEDDEDPLATFORM_H
