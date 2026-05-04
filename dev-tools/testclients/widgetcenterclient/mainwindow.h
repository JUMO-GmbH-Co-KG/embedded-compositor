// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <QMainWindow>

class QLabel;
class EmbeddedShellSurface;

class MainWindow : public QMainWindow {
  Q_OBJECT
  QLabel *m_label = nullptr;

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

private:
  void initShell(EmbeddedShellSurface *, QWindow *);
};
