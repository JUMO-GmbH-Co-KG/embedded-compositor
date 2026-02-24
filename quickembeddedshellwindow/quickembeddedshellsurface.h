// SPDX-License-Identifier: LGPL-3.0-only

#ifndef QUICKEMBEDDEDSHELLSURFACE_H
#define QUICKEMBEDDEDSHELLSURFACE_H

#include "embeddedplatform.h"
#include "quickembeddedshellwindow_global.h"

#include <QQmlParserStatus>
#include <QObject>
#include <QtQml>

class EmbeddedShellSurfaceView;
class EmbeddedShellSurface;

Q_DECLARE_LOGGING_CATEGORY(quickShell)

class EMBEDDEDSHELLWINDOW_EXPORT QuickEmbeddedShellSurface
    : public QObject,
      public QQmlParserStatus
{
  Q_OBJECT
  QML_NAMED_ELEMENT(Surface)

public:
  Q_INTERFACES(QQmlParserStatus)

  QuickEmbeddedShellSurface(QObject *parent = nullptr);

  Q_PROPERTY(QWindow *window READ window WRITE setWindow NOTIFY windowChanged)
  Q_PROPERTY(int implicitWidth READ implicitWidth WRITE setImplicitWidth NOTIFY implicitWidthChanged)
  Q_PROPERTY(int implicitHeight READ implicitHeight WRITE setImplicitHeight NOTIFY implicitHeightChanged)
  Q_PROPERTY(EmbeddedShellTypes::Anchor anchor READ anchor WRITE setAnchor NOTIFY anchorChanged)
  Q_PROPERTY(int margin READ margin WRITE setMargin NOTIFY marginChanged)
  Q_PROPERTY(unsigned int sortIndex READ sortIndex WRITE setSortIndex NOTIFY sortIndexChanged)
  Q_PROPERTY(bool completed READ completed NOTIFY completedChanged)

  void classBegin() override;
  void componentComplete() override;

  EmbeddedShellTypes::Anchor anchor() const;
  void setAnchor(EmbeddedShellTypes::Anchor newAnchor);

  QWindow *window() const;
  void setWindow(QWindow *window);

  int implicitWidth() const;
  void setImplicitWidth(int implicitWidth);

  int implicitHeight() const;
  void setImplicitHeight(int implicitHeight);

  int margin() const;
  void setMargin(int newMargin);

  unsigned int sortIndex() const;
  void setSortIndex(unsigned int sortIndex);

  bool completed() const;

  Q_INVOKABLE EmbeddedShellSurfaceView *createView(const QString &appId,
                                                   const QString &appLabel,
                                                   const QString &label,
                                                   unsigned int sort_index);

  Q_INVOKABLE EmbeddedShellSurfaceView *createView(const QString &appId,
                                                   const QString &appLabel,
                                                   const QString &appIcon,
                                                   const QString &label,
                                                   const QString &icon,
                                                   uint32_t sort_index);

signals:
  void windowChanged(QWindow *window);
  void implicitWidthChanged(int implicitWidth);
  void implicitHeightChanged(int implicitHeight);
  void anchorChanged(EmbeddedShellTypes::Anchor anchor);
  void marginChanged(int margin);
  void sortIndexChanged(unsigned int sortIndex);
  void completedChanged(bool completed);

private:
  QWindow *m_window;
  QSize m_size;
  EmbeddedShellTypes::Anchor m_anchor = EmbeddedShellTypes::Anchor::Undefined;
  EmbeddedShellSurface *m_surface;
  int m_margin;
  unsigned int m_sortIndex;
  bool m_componentComplete;
};

#endif // QUICKEMBEDDEDSHELLSURFACE_H
