// SPDX-License-Identifier: LGPL-3.0-only

#ifndef QUICKEMBEDDEDSHELLWINDOW_H
#define QUICKEMBEDDEDSHELLWINDOW_H

#include "embeddedplatform.h"
#include "quickembeddedshellsurface.h"
#include "quickembeddedshellwindow_global.h"

#include <QLoggingCategory>
#include <QQuickWindow>

class EmbeddedShellSurfaceView;
class EmbeddedShellSurface;

Q_DECLARE_LOGGING_CATEGORY(quickShell)

class EMBEDDEDSHELLWINDOW_EXPORT QuickEmbeddedShellWindow
    : public QQuickWindow,
      public QQmlParserStatus
{
  Q_OBJECT
  QML_NAMED_ELEMENT(Window)

public:
  Q_INTERFACES(QQmlParserStatus)

  QuickEmbeddedShellWindow(QWindow *parent = nullptr);
  ~QuickEmbeddedShellWindow() override;

  Q_PROPERTY(QuickEmbeddedShellSurface *surface READ surface CONSTANT)

  // QQmlParserStatus interface
  void classBegin() override;
  void componentComplete() override;

  QuickEmbeddedShellSurface *surface();

private:
  QuickEmbeddedShellSurface m_surface;
};

#endif // QUICKEMBEDDEDSHELLWINDOW_H
