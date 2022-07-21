#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class MainWindow : public QMainWindow {
  Q_OBJECT
  QLabel *m_label = nullptr;

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  bool event(QEvent *event) override;
private slots:
  void initShell();
};
#endif // MAINWINDOW_H
