// SPDX-License-Identifier: LGPL-3.0-only

#include "quickembeddedshellwindow.h"
#include "embeddedshellsurface.h"

QuickEmbeddedShellWindow::QuickEmbeddedShellWindow(QWindow *parent)
    : QQuickWindow(parent)
{
}

QuickEmbeddedShellWindow::~QuickEmbeddedShellWindow() {}

void QuickEmbeddedShellWindow::classBegin()
{
}

void QuickEmbeddedShellWindow::componentComplete()
{
  m_surface.setWindow(this);
  m_surface.componentComplete();
}

QuickEmbeddedShellSurface *QuickEmbeddedShellWindow::surface()
{
  return &m_surface;
}
