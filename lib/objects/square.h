#ifndef SQUARE_H
#define SQUARE_H

#include "shape.h"
struct Square : Shape {
    float width;
    Square(
        const Point vertices[4],
        const Color &color_,
        const float &mass_,
        const Point &initialVelocity = Point(0, 0),
        const float &initialRotation = 0,
        const Point &initialForce = Point(0, 0),
        const float &initialAngularVelocity = 0,
        const float &initialTorque = 0
    )       : Shape(Point::averageOfArray(vertices, 4), 
            color_, mass_, initialVelocity, initialRotation, initialForce,
            initialAngularVelocity, initialTorque){
        for(int i = 0; i < 4; i++){
            verts.push_back(vertices[i]);
        }
        finalize();
    }
    Square(
        const Point &center,
        const float &width_,
        const Color &color_,
        const float &mass_,
        const Point &initialVelocity = Point(0, 0),
        const float &initialRotation = 0,
        const Point &initialForce = Point(0, 0),
        const float &initialAngularVelocity = 0,
        const float &initialTorque = 0
    )       : width(width_), Shape(center,
            color_, mass_, initialVelocity, initialRotation, initialForce,
            initialAngularVelocity, initialTorque){
        verts = Util::constructSquare(center, width);
        finalize();
    }
};

#endif
