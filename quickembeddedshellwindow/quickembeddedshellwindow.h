// SPDX-License-Identifier: LGPL-3.0-only

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
  // Changes "size" but using "implicit" API style to be consistent with what QtQuick usually does for "wanted size".
  Q_PROPERTY(int implicitWidth READ implicitWidth WRITE setImplicitWidth NOTIFY implicitWidthChanged)
  Q_PROPERTY(int implicitHeight READ implicitHeight WRITE setImplicitHeight NOTIFY implicitHeightChanged)
  Q_PROPERTY(EmbeddedShellTypes::Anchor anchor READ anchor WRITE setAnchor
                 NOTIFY anchorChanged)
  Q_PROPERTY(int margin READ margin WRITE setMargin NOTIFY marginChanged)
  Q_PROPERTY(
     unsigned int sortIndex READ sortIndex WRITE setSortIndex NOTIFY sortIndexChanged)

  EmbeddedShellTypes::Anchor anchor() const;
  void setAnchor(EmbeddedShellTypes::Anchor newAnchor);

  // QQmlParserStatus interface
  void classBegin() override;
  void componentComplete() override;

  int implicitWidth() const;
  void setImplicitWidth(int implicitWidth);
  int implicitHeight() const;
  void setImplicitHeight(int implicitHeight);
  int margin() const;
  void setMargin(int newMargin);
  unsigned int sortIndex() const;
  void setSortIndex(unsigned int sortIndex);

public slots:
  EmbeddedShellSurfaceView *createView(const QString &appId, const QString &appLabel, const QString &label, unsigned int sort_index);
  EmbeddedShellSurfaceView *createView(const QString &appId,
                                       const QString &appLabel,
                                       const QString &appIcon,
                                       const QString &label,
                                       const QString &icon,
                                       uint32_t sort_index);

signals:
  void implicitWidthChanged(int implicitWidth);
  void implicitHeightChanged(int implicitHeight);
  void anchorChanged(EmbeddedShellTypes::Anchor anchor);
  void marginChanged(int margin);
  void sortIndexChanged(unsigned int sortIndex);

private:
  QSize m_size;
  EmbeddedShellTypes::Anchor m_anchor = EmbeddedShellTypes::Anchor::Undefined;
  EmbeddedShellSurface *m_surface = nullptr;
  int m_margin = -1;
  unsigned int m_sortIndex = 0;
  bool m_componentComplete = false;
};

#endif // QUICKEMBEDDEDSHELLWINDOW_H
