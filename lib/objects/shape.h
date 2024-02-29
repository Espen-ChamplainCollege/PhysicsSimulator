#ifndef SHAPE_H
#define SHAPE_H

#include "../../bin/point.h"
#include "../../bin/color.h"
#include <vector>

struct Shape {
    Point position;
    Point velocity;
    Point force;
    float torque;
    float mass;
    float rotation;
    float angularVelocity;
    Color color;
    std::vector<Point> verts;
    Shape(
        const Point &position_,
        const Color &color_,
        const Point &initialVelocity = Point(0, 0),
        const float &initialRotation = 0,
        const Point &initialForce = Point(0, 0),
        const float &initialAngularVelocity = 0,
        const float &initialTorque = 0
        ) : position(position_), color(color_), velocity(initialVelocity),
            rotation(initialRotation), force(initialForce), angularVelocity(initialAngularVelocity),
            torque(initialTorque){};
};

#endif
