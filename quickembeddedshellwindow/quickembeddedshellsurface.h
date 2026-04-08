// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "embeddedplatform.h"
#include "quickembeddedshellwindow_global.h"
#include "embeddedshellsurfaceview.h"

#include <QQmlParserStatus>
#include <QObject>
#include <QtQml>

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
  Q_PROPERTY(bool completed READ completed NOTIFY completedChanged)
  Q_PROPERTY(bool visible READ visible NOTIFY visibleChanged)

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

  bool completed() const;

  bool visible() const;

  Q_INVOKABLE EmbeddedShellSurfaceView *createView(const QString &label,
                                                   const QString &icon,
                                                   uint32_t sortIndex,
                                                   const QVariant &customData = QVariant(),
                                                   EmbeddedShellSurfaceView *parentView = nullptr);

signals:
  void windowChanged();
  void implicitWidthChanged();
  void implicitHeightChanged();
  void anchorChanged();
  void marginChanged();
  void completedChanged();
  void visibleChanged();

private:
  QWindow *m_window;
  QSize m_size;
  EmbeddedShellTypes::Anchor m_anchor = EmbeddedShellTypes::Anchor::Undefined;
  EmbeddedShellSurface *m_surface;
  int m_margin;
  bool m_componentComplete;
  bool m_visible;
};
