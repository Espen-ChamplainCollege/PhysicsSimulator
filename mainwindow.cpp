#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qpainter.h"
#include <qnamespace.h>
#include <qwindowdefs.h>
#include <qevent.h>
#include "sphere.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setMouseTracking(true);
  QPalette palette = QPalette();
  palette.setColor(QPalette::Window, QColor::fromRgb(20, 24, 29));
  this->setAutoFillBackground(true);
  this->setPalette(palette);

  //sphere = new Sphere(Point(50, 50), Point(2.5, 2.5), Color(255, 255, 255), 25);
  sandbox = new Sandbox(this->width(), this->height(), std::chrono::duration<double, std::milli>(16));
  sandbox->addButtons();
  
  sandbox->start().detach();
  this->startTimer(16);
}

MainWindow::~MainWindow(){
  delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event){
  QPainter painter(this);                         // Make a painter. "this" is the window.
  painter.setRenderHint(QPainter::Antialiasing);  // Turn on antialiasing

  
  painter.setPen(QPen(Qt::transparent));
  for(int i = 0; i < sandbox->spheres.size(); i++){
    painter.setBrush(QColor(sandbox->spheres[i]->color));
    painter.drawEllipse(
      sandbox->spheres[i]->position.x, 
      sandbox->spheres[i]->position.y, 
      sandbox->spheres[i]->radius, 
      sandbox->spheres[i]->radius
    );  // Draw ellipse (x, y, width, height) 
  }



  painter.setPen(QPen(Qt::transparent));
  for(int i = 0; i < sandbox->triangles.size(); i++) {
      static const QPointF points[3] = {
          QPointF(50.0, 50.0),
          QPointF(50.0, 50.0),
          QPointF(50.0, 50.0)
      };
      painter.setBrush(QColor(sandbox->triangles[i]->color));
      painter.drawPolygon(points, 3, Qt::OddEvenFill);
  }

  // We draw the buttons/UI second because it needs to be on top.
  painter.setPen(QPen(Qt::white));
  painter.setBrush(QColor::fromRgb(20, 24, 29));
  QBrush b(palette().window());
  for(auto i = sandbox->buttons.begin(); i != sandbox->buttons.end(); i++){
    painter.fillRect(
      i->first.pos.x,
      i->first.pos.y,
      i->first.width,
      i->first.height,
      b
    );
    painter.drawRect(
      i->first.pos.x,
      i->first.pos.y,
      i->first.width,
      i->first.height
    );
    painter.setFont(QFont("Arial"));
    QRect rect = painter.boundingRect(
      i->first.pos.x, i->first.pos.y, 
      i->first.width, i->first.height, 
      Qt::AlignHCenter | Qt::AlignVCenter, 
      i->first.text.c_str());
    painter.drawText(rect, i->first.text.c_str());
  }
  
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event){
  sandbox->tryClickButtons(event->pos());
}

void MainWindow::timerEvent(QTimerEvent *event) { repaint(); }
