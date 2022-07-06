#ifndef EMBEDDEDSHELLWINDOW_H
#define EMBEDDEDSHELLWINDOW_H

#include "embeddedshellwindow_global.h"
#include <QQuickWindow>

class EMBEDDEDSHELLWINDOW_EXPORT QuickEmbeddedShellWindow : public QQuickWindow
{
    Q_OBJECT
    uint m_anchor;

public:
    QuickEmbeddedShellWindow(QWindow* parent = nullptr);
    ~QuickEmbeddedShellWindow() override;
    Q_PROPERTY(uint anchor READ anchor WRITE setAnchor NOTIFY anchorChanged)
    uint anchor() const;
    void setAnchor(uint newAnchor);

    static void registerQml() {
        qmlRegisterType<QuickEmbeddedShellWindow>("com.embeddedcompositor.window", 1, 0, "EmbeddedShellWindow");
    }
signals:
    void anchorChanged();
};

#endif // EMBEDDEDSHELLWINDOW_H
