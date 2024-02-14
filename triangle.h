#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "point.h"
#include "color.h"

struct Triangle {
    Point position;
    Point velocity;
    float radius;
    Color color;
    Point verts[3];
    Triangle(const Point &p, const Color &c, float r) : position(p), color(c), radius(r){
        verts[0] = p;
        verts[1] = Point(p.x + radius, p.y + radius);
        verts[2] = Point(p.x, p.y + radius);
          };
    Triangle(const Point &p, const Point &v, const Color &c, float r) : position(p), velocity(v), color(c), radius(r){};
};


#endif // TRIANGLE_H
