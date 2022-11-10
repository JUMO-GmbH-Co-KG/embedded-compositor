#ifndef QUICKEMBEDDEDSHELLWINDOW_H
#define QUICKEMBEDDEDSHELLWINDOW_H

#include "embeddedplatform.h"
#include "quickembeddedshellwindow_global.h"
#include <QLoggingCategory>
#include <QQuickWindow>

class EmbeddedShellSurfaceView;
class EmbeddedShellSurface;

Q_DECLARE_LOGGING_CATEGORY(quickShell)

class EMBEDDEDSHELLWINDOW_EXPORT QuickEmbeddedShellWindow
    : public QQuickWindow,
      public QQmlParserStatus {
  Q_OBJECT

public:
  Q_INTERFACES(QQmlParserStatus)

  QuickEmbeddedShellWindow(QWindow *parent = nullptr);
  ~QuickEmbeddedShellWindow() override;
  Q_PROPERTY(EmbeddedShellTypes::Anchor anchor READ anchor WRITE setAnchor
                 NOTIFY anchorChanged)
  Q_PROPERTY(int margin READ margin WRITE setMargin NOTIFY marginChanged)
  Q_PROPERTY(
      int sortIndex READ sortIndex WRITE setSortIndex NOTIFY sortIndexChanged)

  EmbeddedShellTypes::Anchor anchor() const;
  void setAnchor(EmbeddedShellTypes::Anchor newAnchor);

  // QQmlParserStatus interface
  void classBegin() override;
  void componentComplete() override;

  int margin() const;
  void setMargin(int newMargin);
  int sortIndex() const;
  void setSortIndex(int sortIndex);

public slots:
  EmbeddedShellSurfaceView *createView(QString label, int sort_index);

signals:
  void anchorChanged(EmbeddedShellTypes::Anchor anchor);
  void marginChanged(int margin);
  void sortIndexChanged(int sortIndex);

private:
  EmbeddedShellTypes::Anchor m_anchor = EmbeddedShellTypes::Anchor::Undefined;
  EmbeddedShellSurface *m_surface = nullptr;
  int m_margin = -1;
  int m_sortIndex = 0;
};

#endif // QUICKEMBEDDEDSHELLWINDOW_H
