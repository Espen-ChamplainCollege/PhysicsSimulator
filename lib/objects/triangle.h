#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../../bin/point.h"
#include "../../bin/color.h"
#include "shape.h"
#include "../../bin/util.h"
#include <QPointF>

struct Triangle : Shape {
    float length;
    // This is such a disaster to format. I'm gonna leave it to Espen
    // for when he goes through to clean up the files. 
    // (Maybe we dont need all of these parameters in the constructor)
    Triangle(
        const Point vertices[3],
        const Color &color_,
        const float &mass_,
        const Point &initialVelocity = Point(0, 0),
        const float &initialRotation = 0,
        const Point &initialForce = Point(0, 0),
        const float &initialAngularVelocity = 0,
        const float &initialTorque = 0
        ) : Shape(
            Point::averageOfArray(vertices, 3), // average of 3 points = center of triangle
            color_, mass_, initialVelocity,
            initialRotation, initialForce,
            initialAngularVelocity, initialTorque){
                for(int i = 0; i < 3; i++){
                    verts.push_back(vertices[i]);
                }
                finalize();
            }
    Triangle(
        const Point &center,
        const float length_,
        const Color &color_,
        const float &mass_,
        const Point &initialVelocity = Point(0, 0),
        const float &initialRotation = 0,
        const Point &initialForce = Point(0, 0),
        const float &initialAngularVelocity = 0,
        const float &initialTorque = 0
        ) : length(length_), Shape(
            center,
            color_, mass_, initialVelocity,
            initialRotation, initialForce,
            initialAngularVelocity, initialTorque){
                verts = Util::constructTriangle(center, length);
                finalize();
            }
};


#endif // TRIANGLE_H
