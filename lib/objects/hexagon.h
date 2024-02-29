#ifndef HEXAGON_H
#define HEXAGON_H


#include "../../bin/point.h"
#include "../../bin/color.h"
#include "shape.h"
#include "../../bin/util.h"

struct Hexagon : Shape {
    float width;
    Hexagon(
        const Point vertices[6],
        const Color &color_,
        const float &mass_,
        const Point &initialVelocity = Point(0, 0),
        const float &initialRotation = 0,
        const Point &initialForce = Point(0, 0),
        const float &initialAngularVelocity = 0,
        const float &initialTorque = 0,
        const float &initialInertiaTensor = 1
        ) : Shape(Point::averageOfArray(vertices, 6),
            color_, mass_, initialVelocity, initialRotation, initialForce,
            initialAngularVelocity, initialTorque, initialInertiaTensor){

            for(int i = 0; i < 6; i++){
            verts.push_back(vertices[i]);
            }
    }
    Hexagon(
        const Point &center,
        const float &width_,
        const Color &color_,
        const float &mass_,
        const Point &initialVelocity = Point(0, 0),
        const float &initialRotation = 0,
        const Point &initialForce = Point(0, 0),
        const float &initialAngularVelocity = 0,
        const float &initialTorque = 0,
        const float &initialInertiaTensor = 1
        ) : width(width_), Shape(center,
            color_, mass_, initialVelocity, initialRotation, initialForce,
            initialAngularVelocity, initialTorque, initialInertiaTensor){
            verts = Util::constructHexagon(center, width);
            }
};


#endif
