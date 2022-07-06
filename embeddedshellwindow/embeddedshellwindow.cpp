#include "embeddedshellwindow.h"


QuickEmbeddedShellWindow::QuickEmbeddedShellWindow(QWindow* parent)
    :QQuickWindow(parent)
{
qDebug()<<__PRETTY_FUNCTION__;
}

QuickEmbeddedShellWindow::~QuickEmbeddedShellWindow()
{
}

uint QuickEmbeddedShellWindow::anchor() const
{
    qDebug()<<__PRETTY_FUNCTION__<<m_anchor;
    return m_anchor;
}

void QuickEmbeddedShellWindow::setAnchor(uint newAnchor)
{
    qDebug()<<__PRETTY_FUNCTION__<<newAnchor;
    if (m_anchor == newAnchor)
        return;
    m_anchor = newAnchor;
    emit anchorChanged();
}
