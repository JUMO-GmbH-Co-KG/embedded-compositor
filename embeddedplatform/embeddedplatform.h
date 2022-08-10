#ifndef EMBEDDEDPLATFORM_H
#define EMBEDDEDPLATFORM_H
#include <QObject>

class EmbeddedShellSurface;
class QWindow;

class EmbeddedPlatform : public QObject {
  Q_OBJECT
public:
  EmbeddedPlatform();
  ~EmbeddedPlatform() override;
  enum class Anchor {
    Undefined = 0,
    Top = 1,
    Bottom = 2,
    Left = 3,
    Right = 4,
    Center = 5
  };
  Q_ENUM(Anchor)

  static EmbeddedShellSurface *shellSurfaceForWindow(QWindow *window);
};
Q_DECLARE_METATYPE(EmbeddedPlatform::Anchor)

#endif // EMBEDDEDPLATFORM_H
