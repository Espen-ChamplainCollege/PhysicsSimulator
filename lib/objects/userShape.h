#ifndef USERSHAPE_H
#define USERSHAPE_H


#include "../../bin/point.h"
#include "../../bin/color.h"
#include "shape.h"
#include "../../bin/util.h"

struct UserShape : Shape {
    float width;
    UserShape(
        // made to work with 5 points for now
        const Point vertices[5],
        const Color& color_,
        const float& mass_,
        const Point& initialVelocity = Point(0, 0),
        const float& initialRotation = 0,
        const Point& initialForce = Point(0, 0),
        const float& initialAngularVelocity = 0,
        const float& initialTorque = 0
        ) : Shape(Point::averageOfArray(vertices, 5),
            color_, mass_, initialVelocity, initialRotation, initialForce,
            initialAngularVelocity, initialTorque) {

            for (int i = 0; i < 5; i++) {
                verts.push_back(vertices[i]);
            }
            finalize();
    }
    UserShape(
        const Point& center,
        const float& width_,
        const Color& color_,
        const float& mass_,
        const std::vector<Point> &vertices,
        const Point& initialVelocity = Point(0, 0),
        const float& initialRotation = 0,
        const Point& initialForce = Point(0, 0),
        const float& initialAngularVelocity = 0,
        const float& initialTorque = 0
        ) : width(width_), Shape(center,
            color_, mass_, initialVelocity, initialRotation, initialForce,
            initialAngularVelocity, initialTorque){
            verts = vertices;
            
            finalize();
    }
};


#endif
