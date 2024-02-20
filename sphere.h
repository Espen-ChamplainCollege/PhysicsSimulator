#ifndef SPHERE_H
#define SPHERE_H


#include "point.h"
#include "color.h"

struct Sphere {
  Point position;
  Point velocity;
  float radius;
  float rotation;
  float angularVelocity;
  Color color;
  Sphere(const Point &p, const Color &c, float r) : position(p), color(c), radius(r){};
  Sphere(const Point &p, const Point &v, const Color &c, float r) : position(p), velocity(v), color(c), radius(r){};

};


#endif
