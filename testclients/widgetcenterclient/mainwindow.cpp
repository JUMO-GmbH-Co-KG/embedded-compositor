#include "mainwindow.h"
#include "embeddedplatform.h"
#include "embeddedshellsurface.h"
#include <QDebug>
#include <QLabel>
#include <QWindow>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  m_label = new QLabel("initial state", this);
  this->setCentralWidget(m_label);

  connect(EmbeddedPlatform::instance(), &EmbeddedPlatform::shellSurfaceCreated,
          this, &MainWindow::initShell);

  //  windowHandle()->setProperty(
  //      "anchor", QVariant::fromValue(EmbeddedPlatform::Anchor::Center));
}

MainWindow::~MainWindow() {}

void MainWindow::initShell(EmbeddedShellSurface *shellSurface,
                           QWindow *window) {
  if (this->windowHandle() != window) {
    return;
  }
  if (shellSurface == nullptr) {
    qWarning() << "NO SHELL SURFACE!";
    return;
  }
  shellSurface->sendAnchor(EmbeddedShellTypes::Anchor::Center);
  auto v1 = shellSurface->createView("View One", 3);
  auto v2 = shellSurface->createView("View Two", 2);
  auto v3 = shellSurface->createView("View Three", 1);
  connect(v1, &EmbeddedShellSurfaceView::selected, this,
          [=] { m_label->setText(v1->label()); });
  connect(v2, &EmbeddedShellSurfaceView::selected, this,
          [=] { m_label->setText(v2->label()); });
  connect(v3, &EmbeddedShellSurfaceView::selected, this,
          [=] { m_label->setText(v3->label()); });
}
