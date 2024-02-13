#ifndef PENTAGON_H
#define PENTAGON_H

#include "point.h"
#include "color.h"
#include "qpolygon.h"


class pentagon
{
public:
    pentagon(Point center, double radius, Color color);
    double points[10];
    Point velocity;
    QPolygon regPentagon;
};

#endif // PENTAGON_H
