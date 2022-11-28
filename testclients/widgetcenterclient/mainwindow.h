// SPDX-License-Identifier: LGPL-3.0-only

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class EmbeddedShellSurface;

class MainWindow : public QMainWindow {
  Q_OBJECT
  QLabel *m_label = nullptr;

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;
private slots:
  void initShell(EmbeddedShellSurface *, QWindow *);
};
#endif // MAINWINDOW_H
