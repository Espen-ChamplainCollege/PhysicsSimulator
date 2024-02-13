#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "point.h"
#include "color.h"

struct Triangle {
    Point position;
    Point velocity;
    float radius;
    Color color;
    Triangle(const Point &p, const Color &c) : position(p), color(c){};
    Triangle(const Point &p, const Point &v, const Color &c) : position(p), velocity(v), color(c){};

};


#endif // TRIANGLE_H
