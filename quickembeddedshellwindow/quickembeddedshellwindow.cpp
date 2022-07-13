#include "quickembeddedshellwindow.h"
#include "quickembeddedshell.h"

QuickEmbeddedShellWindow::QuickEmbeddedShellWindow(QWindow* parent)
    :QQuickWindow(parent)
{
qDebug()<<__PRETTY_FUNCTION__;
}

QuickEmbeddedShellWindow::~QuickEmbeddedShellWindow()
{
}

QuickEmbeddedShellWindow::Anchor QuickEmbeddedShellWindow::anchor() const
{
    qDebug()<<__PRETTY_FUNCTION__<<m_anchor;
    return m_anchor;
}

void QuickEmbeddedShellWindow::setAnchor(Anchor newAnchor)
{
    qDebug()<<__PRETTY_FUNCTION__<<m_anchor<<"->"<<newAnchor;
    if (m_anchor == newAnchor)
        return;
    m_anchor = newAnchor;
    emit anchorChanged(newAnchor);
}

void QuickEmbeddedShell::registerTypes(const char *uri)
{
    qmlRegisterType<QuickEmbeddedShellWindow>(uri, 1, 0, "Window");
}

