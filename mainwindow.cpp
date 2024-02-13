#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qpainter.h"
#include <qnamespace.h>
#include "sphere.h"
#include "qpolygon.h"


Sphere *sphere;
QPolygon polygon;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  QPalette palette = QPalette();
  palette.setColor(QPalette::Window, QColor::fromRgb(20, 24, 29));
  this->setAutoFillBackground(true);
  this->setPalette(palette);

  sphere = new Sphere(Point(50, 50), Point(2.5, 2.5), Color(255, 255, 255), 25);
  polygon << QPoint(100,100) << QPoint(70,130) << QPoint(85,160) << QPoint(115,160) << QPoint(130,130);

  this->startTimer(16); // 16 milliseconds roughly = 60fps
}

MainWindow::~MainWindow(){
  delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event){

  QPainter painter(this);                         // Make a painter. "this" is the window.
  painter.setRenderHint(QPainter::Antialiasing);  // Turn on antialiasing
  painter.setPen(QPen(Qt::transparent));          // Turn off border
  painter.setBrush(QColor(sphere->color)); // set the brush. From my understanding Brush = filled, Pen = outlined.
  
  painter.drawEllipse(sphere->position.x, sphere->position.y, sphere->radius, sphere->radius);  // Draw ellipse (x, y, width, height) 
  // In QT, (0, 0) is the top left corner. As y increases, the position moves down. As x increases, the position moves right.

  painter.drawPolygon(polygon);

}

// The update loop
void MainWindow::timerEvent(QTimerEvent *event) {
  Point nextPos = sphere->position + sphere->velocity;
  if(nextPos.y + sphere->radius > this->height() || nextPos.y < 0){
    sphere->velocity.y *= -1;
  }
  if(nextPos.x + sphere->radius > this->width() || nextPos.x < 0){
    sphere->velocity.x *= -1;
  }
  sphere->position += sphere->velocity;
  repaint();
}
