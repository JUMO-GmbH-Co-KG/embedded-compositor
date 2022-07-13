#ifndef QUICKEMBEDDEDSHELLWINDOW_H
#define QUICKEMBEDDEDSHELLWINDOW_H

#include "quickembeddedshellwindow_global.h"
#include <QQuickWindow>

class EMBEDDEDSHELLWINDOW_EXPORT QuickEmbeddedShellWindow : public QQuickWindow
{
    Q_OBJECT
public:
    enum class Anchor {
        Undefined = 0,
        Top = 1,
        Bottom = 2,
        Left = 3,
        Right = 4,
        Center = 5
    };
    Q_ENUM(Anchor)

    QuickEmbeddedShellWindow(QWindow* parent = nullptr);
    ~QuickEmbeddedShellWindow() override;
    Q_PROPERTY(Anchor anchor READ anchor WRITE setAnchor NOTIFY anchorChanged)
    Anchor anchor() const;
    void setAnchor(Anchor newAnchor);

signals:
    void anchorChanged(Anchor anchor);

private:
    Anchor m_anchor;
};

#endif // QUICKEMBEDDEDSHELLWINDOW_H
