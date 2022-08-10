#include "mainwindow.h"
#include "embeddedplatform.h"
#include "embeddedshellsurface.h"
#include <QDebug>
#include <QEvent>
#include <QLabel>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  qDebug() << __PRETTY_FUNCTION__;
  m_label = new QLabel("initial state", this);
  this->setCentralWidget(m_label);
//  initShell();
}

MainWindow::~MainWindow() {}

void MainWindow::initShell() {
  qDebug() << __PRETTY_FUNCTION__;
  Q_ASSERT(this->windowHandle() != nullptr);
  auto shellSurface =
      EmbeddedPlatform::shellSurfaceForWindow(this->windowHandle());
  qDebug() << "shellSurface" << shellSurface;
  if (shellSurface == nullptr) {
    qWarning() << "NO SHELL SURFACE!";
    return;
  }
  shellSurface->sendAnchor(EmbeddedShellSurface::Anchor::Center);
  auto v1 = shellSurface->createView("View One");
  auto v2 = shellSurface->createView("View Two");
  auto v3 = shellSurface->createView("View Three");
  connect(v1, &EmbeddedShellSurfaceView::selected, this,
          [=] { m_label->setText(v1->label()); });
  connect(v2, &EmbeddedShellSurfaceView::selected, this,
          [=] { m_label->setText(v2->label()); });
  connect(v3, &EmbeddedShellSurfaceView::selected, this,
          [=] { m_label->setText(v3->label()); });
}

bool MainWindow::event(QEvent *event) {
    qDebug()<<"EVENT"<<event->type()<<windowHandle() << EmbeddedPlatform::shellSurfaceForWindow(this->windowHandle());
  // FIXME HACK what is the right way to handle shell surface creation?
  // found this event happens just after shell surface creation
  if (event->type() == QEvent::ShowToParent)
    initShell();
  return QMainWindow::event(event);
}
