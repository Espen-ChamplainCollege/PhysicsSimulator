#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qevent.h>
#include <qwidget.h>
#include "sandbox.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow { 
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

protected:
  void paintEvent(QPaintEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void timerEvent(QTimerEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
private:
  Ui::MainWindow *ui;
  Sandbox *sandbox;
  Point cameraPos = Point(0, 0);
  Point cameraScale = Point(1, 1);
};

#endif // MAINWINDOW_H
