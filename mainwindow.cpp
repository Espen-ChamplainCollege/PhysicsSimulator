#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qpainter.h"
#include <qnamespace.h>
#include <qwindowdefs.h>
#include <qevent.h>
#include "sphere.h"

#include "triangle.h"



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
  // this is how you rotate something:
  // 1. reset the transform
  // 2. center the transform on the object
  // 3. rotate the transform
  // 4. uncenter the transform on the object

  for(int i = 0; i < sandbox->triangles.size(); i++) {
    painter.setBrush(QColor(sandbox->triangles[i]->color));
    painter.resetTransform();
    Point triangleCenter = Point(0, 0);
    for(int k = 0; k < 3; k++){
      triangleCenter += Point(sandbox->triangles[i]->points[k]);
    }
    triangleCenter /= 3;
    painter.translate(QPoint(triangleCenter));
    painter.rotate(sandbox->triangles[i]->rotation);
    painter.translate(-QPoint(triangleCenter));
    painter.drawPolygon(
      sandbox->triangles[i]->points,
      3
    );
    // this triangle rotation stuff is a workaround because the triangle position is not the triangle's center.
    // will fix later
  }
  painter.resetTransform();

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
