#ifndef SHAPE_H
#define SHAPE_H

#include "../../bin/point.h"
#include "../../bin/color.h"
#include <vector>
#include <iostream>

struct Shape {
    Point position;
    Point velocity;
    Point force;
    float torque;
    float mass;
    float inertiaTensor;
    float rotation;
    float angularVelocity;
    Color color;
    std::vector<Point> verts;
    Shape(
        const Point &position_,
        const Color &color_,
        const float &mass_,
        const Point &initialVelocity = Point(0, 0),
        const float &initialRotation = 0,
        const Point &initialForce = Point(0, 0),
        const float &initialAngularVelocity = 0,
        const float &initialTorque = 0,
        const float &initialInertiaTensor = 1
        ) : position(position_), color(color_), mass(mass_), velocity(initialVelocity),
            rotation(initialRotation), force(initialForce), angularVelocity(initialAngularVelocity),
            torque(initialTorque), inertiaTensor(initialInertiaTensor){};
    
    const void printInfo() const {
        std::cout << "Shape at position: " << position << std::endl;
        std::cout << "\tVelocity: " << velocity 
            << "\n\tForce: " << force << "\n\tRotation: " << rotation 
            << "\n\tTorque: " << torque << "\n\tMass: " << std::to_string(mass)
            << "\n\tAngular velocity: " << angularVelocity 
            << "\n\tInertia tensor: " << inertiaTensor 
            << "\n\tColor: " << color << "\n\tVerts:";
        for(int i = 0; i < verts.size(); i++){
            std::cout << "\n\t\t" << verts[i];
        }
        std::cout << std::endl;
    }
};

#endif
