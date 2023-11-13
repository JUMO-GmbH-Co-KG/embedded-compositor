#ifndef WAYLANDINPUTREGION_H
#define WAYLANDINPUTREGION_H

#include <QQuickItem>
#include <QVariant>


/*
QuickItem to set an input region on a window.
pixelMask: renders child elements into a pixmap to create a pixel-perfect input
region at the cost of rendering and wayland protocol overhead
Currently only one InputRegion Item per window is supported.
*/

class WaylandInputRegion : public QQuickItem
{
    Q_OBJECT
public:
    explicit WaylandInputRegion(QQuickItem *parent = nullptr);
    Q_PROPERTY(bool pixelMask READ pixelMask WRITE setPixelMask NOTIFY pixelMaskChanged)
protected:
    void itemChange(ItemChange, const ItemChangeData &) override;
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;

public slots:
    void SetImage(QVariant image, QRect position);
    void updateRegion();

public:
    void componentComplete() override;
    QQuickItem *item() const;
    void setItem(QQuickItem *newItem);
    bool pixelMask() const { return m_pixelMask; }

    void setPixelMask(bool newMask) {
        if (m_pixelMask == newMask)
            return;
        m_pixelMask = newMask;
        emit pixelMaskChanged(m_pixelMask);
    }

signals:
    void pixelMaskChanged(bool mask);

private:
    bool m_pixelMask = false;
};

#endif // WAYLANDINPUTREGION_H
