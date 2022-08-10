#ifndef QUICKEMBEDDEDSHELLWINDOW_H
#define QUICKEMBEDDEDSHELLWINDOW_H

#include "embeddedplatform.h"
#include "quickembeddedshellwindow_global.h"
#include <QQuickWindow>

class EmbeddedShellSurfaceView;
class EmbeddedShellSurface;

class EMBEDDEDSHELLWINDOW_EXPORT QuickEmbeddedShellWindow
    : public QQuickWindow,
      public QQmlParserStatus {
  Q_OBJECT
  using Anchor = EmbeddedPlatform::Anchor;

public:
  Q_INTERFACES(QQmlParserStatus)

  QuickEmbeddedShellWindow(QWindow *parent = nullptr);
  ~QuickEmbeddedShellWindow() override;
  Q_PROPERTY(EmbeddedPlatform::Anchor anchor READ anchor WRITE setAnchor NOTIFY
                 anchorChanged)
  Q_PROPERTY(int margin READ margin WRITE setMargin NOTIFY marginChanged)

  Anchor anchor() const;
  void setAnchor(Anchor newAnchor);

  // QQmlParserStatus interface
  void classBegin() override;
  void componentComplete() override;

  int margin() const;
  void setMargin(int newMargin);

public slots:
  EmbeddedShellSurfaceView *createView(QString label);

signals:
  void anchorChanged(Anchor anchor);

  void marginChanged();

private:
  Anchor m_anchor = Anchor::Undefined;
  EmbeddedShellSurface *m_surface = nullptr;
  int m_margin = -1;
};

#endif // QUICKEMBEDDEDSHELLWINDOW_H
