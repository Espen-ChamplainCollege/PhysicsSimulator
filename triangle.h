#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "point.h"
#include "color.h"
#include <QPointF>

struct Triangle {
    Point position;
    Point velocity;
    QPointF points[3];
    float radius;
    Color color;

    Triangle(const Point &p, const Color &c, float r) : position(p), color(c), radius(r){
        // Temp numbers, pay no mind
        points[0] = QPointF(position.x, position.y);
        points[1] = QPointF(position.x - 8, position.y - 14);
        points[2] = QPointF(position.x + 8, position.y - 14);
    };
    Triangle(const Point &p, const Point &v, const Color &c, float r) : position(p), velocity(v), color(c), radius(r){
        // Temp numbers, pay no mind
        points[0] = QPointF(position.x, position.y);
        points[1] = QPointF(position.x - 8, position.y - 14);
        points[2] = QPointF(position.x + 8, position.y - 14);
    };
};


#endif // TRIANGLE_H
