// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

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
  QML_ELEMENT

public:
  explicit WaylandInputRegion(QQuickItem *parent = nullptr);
  Q_PROPERTY(bool pixelMask READ pixelMask WRITE setPixelMask NOTIFY pixelMaskChanged)
  /**
     * @brief Whether masking is active
     *
     * When set to false, the window mask is reset.
     * Default is true.
     */
  Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)

protected:
  void itemChange(ItemChange, const ItemChangeData &) override;
  void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

public:
  void setImage(QVariant image, QRect position);
  void updateRegion();

public:
  void componentComplete() override;
  QQuickItem *item() const;
  void setItem(QQuickItem *newItem);

  bool pixelMask() const { return m_pixelMask; }
  void setPixelMask(bool newMask);

  bool active() const;
  void setActive(bool active);

signals:
  void pixelMaskChanged(bool mask);
  void activeChanged(bool active);

private:
  bool m_pixelMask = false;
  bool m_active = true;
};
