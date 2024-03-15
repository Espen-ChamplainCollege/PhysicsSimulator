#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qpainter.h"
#include <qnamespace.h>
#include <qwindowdefs.h>
#include <qevent.h>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setMouseTracking(true);
    QPalette palette = QPalette();
    palette.setColor(QPalette::Window, QColor::fromRgb(20, 24, 29));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    this->setWindowTitle("Physics");

    //sphere = new Sphere(Point(50, 50), Point(2.5, 2.5), Color(255, 255, 255), 25);
    sandbox = new Sandbox(this->width(), this->height(), std::chrono::duration<double, std::milli>(16));
    sandbox->addMenu();

    sandbox->start().detach();
    this->startTimer(16);
}

MainWindow::~MainWindow(){
    delete ui;
}
void MainWindow::wheelEvent(QWheelEvent *event){
    cameraScale += ((float)event->angleDelta().y()) / 1000;
}
void MainWindow::keyPressEvent(QKeyEvent *event){
    int hozScrollSpeed = 20;
    int vertScrollSpeed = 20;
    if(event->key() == Qt::Key_Left) cameraPos.x += hozScrollSpeed;
    if(event->key() == Qt::Key_Right) cameraPos.x -= hozScrollSpeed;
    if(event->key() == Qt::Key_Up) cameraPos.y += vertScrollSpeed;
    if(event->key() == Qt::Key_Down) cameraPos.y -= vertScrollSpeed;

    if(event->key() == Qt::Key_Tab) sandbox->paused = !sandbox->paused;

}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);                         // Make a painter. "this" is the window.
    painter.setRenderHint(QPainter::Antialiasing);  // Turn on antialiasing
    painter.translate(cameraPos.x, cameraPos.y);
    painter.scale(cameraScale.x, cameraScale.y);

    painter.setPen(QPen(Qt::white));
    painter.drawRect(sandbox->position.x, sandbox->position.y, sandbox->width, sandbox->height);

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


    for(int i = 0; i < sandbox->shapes.size(); i++){
        Shape *s = sandbox->shapes[i];

        painter.setBrush(QColor(s->color));
        painter.drawPolygon(Util::convertPointVector(s->verts), s->verts.size());
    }

    //painter.resetTransform();

    // We draw the buttons/UI second because it needs to be on top.
    painter.resetTransform();
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

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if ((sandbox->checkMouseClick) && (sandbox->userShapePoints.size() < sandbox->userShapeVerts)) {
        Point mousePos = QWidget::mapFromGlobal(QCursor::pos());
        sandbox->userShapePoints.push_back(mousePos);
    }
}

void MainWindow::timerEvent(QTimerEvent *event) { repaint(); }
