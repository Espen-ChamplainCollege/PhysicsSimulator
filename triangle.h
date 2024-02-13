#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "point.h"
#include "color.h"

struct Triangle {
    Point position;
    Point velocity;
    float radius;
    Color color;
    Triangle(const Point &p, const Color &c, float r) : position(p), color(c), radius(r){};
    Triangle(const Point &p, const Point &v, const Color &c, float r) : position(p), velocity(v), color(c), radius(r){};

};


#endif // TRIANGLE_H


/**
 *  Im not sure where to put the code for painting the triangle
 *  And I dont know how to add a button for it
 *  So Im just gunna leave it here for now
 *
 *  Q_UNUSED(event);
 *  QPainter painter(this);
 *
 *  QPoint points[3] = {
 *      QPoint(width() / 2, 10),
 *      QPoint(10, height() - 10),
 *      QPoint(width() - 10, height() - 10)
 *  };
 *
 *  painter.setBrush(Qt::blue);
 *  painter.drawPolygon(points, 3);
 *
