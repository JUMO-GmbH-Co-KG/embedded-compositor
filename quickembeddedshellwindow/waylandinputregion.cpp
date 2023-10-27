#include "waylandinputregion.h"
#include <QDebug>
#include <QQuickWindow>
#include <private/qwaylandwindow_p.h>
#include <QQuickItemGrabResult>
#include <QBitmap>
#include <QImage>
#include <QFile>
#include <QPainter>

WaylandInputRegion::WaylandInputRegion(QQuickItem* parent): QQuickItem(parent) {
    setFlag(QQuickItem::ItemHasContents);
}

void WaylandInputRegion::itemChange(ItemChange c, const ItemChangeData& d) {
    updateRegion();
    QQuickItem::itemChange(c,d);
}

void WaylandInputRegion::geometryChanged(const QRectF &newGeometry,
                                         const QRectF &oldGeometry) {
  QQuickItem::geometryChanged(newGeometry, oldGeometry);
  updateRegion();
}

void WaylandInputRegion::SetImage(QVariant image, QRect sceneRect)
{
  auto img = image.value<QImage>();
  img = img.createAlphaMask(Qt::AvoidDither);
  auto bitmap = QBitmap::fromImage(img, Qt::ThresholdDither|Qt::AvoidDither);
  QRegion region(bitmap);
  region.translate(sceneRect.topLeft());

  if(window() == nullptr) {
      return;
  }
  window()->handle()->setMask(region);
}

void WaylandInputRegion::updateRegion() {

  if(window() == nullptr) {
      return;
  }
  // if we have no item to create a mask from, we can just use ourselves as a rectangle
  auto mapped = mapRectToScene(boundingRect());
  auto aligned = mapped.toAlignedRect();

  if (!m_pixelMask) {
      QRegion region(aligned.intersected(window()->geometry()));
      window()->handle()->setMask(region);
      return;
  }

  auto result = grabToImage(aligned.size());
  connect(result.data(), &QQuickItemGrabResult::ready, this, [=](){
      this->SetImage(result->image(), aligned);
    });
}

void WaylandInputRegion::componentComplete()
{
  updateRegion();
  QQuickItem::componentComplete();
}
