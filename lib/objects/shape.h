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
    float mass, inverseMass;
    float inertiaTensor, inverseI;
    float rotation;
    float angularVelocity;
    float friction;
    Color color;
    std::size_t id;
    std::vector<Point> verts;
    bool isStatic = false;
    std::vector<std::vector<Point>> triangles;
    Shape(
        const Point &position_,
        const Color &color_,
        const float &mass_,
        const Point &initialVelocity = Point(0, 0),
        const float &initialRotation = 0,
        const Point &initialForce = Point(0, 0),
        const float &initialAngularVelocity = 0,
        const float &initialTorque = 0
        ) : position(position_), color(color_), mass(mass_), velocity(initialVelocity),
            rotation(initialRotation), force(initialForce), angularVelocity(initialAngularVelocity),
            torque(initialTorque), inverseMass(1.0 / mass_){
        id = internalIdCount;
        internalIdCount++;
    };

    const void calculateInertiaTensor(){
        if(verts.size() == 0) return;
        inertiaTensor = 0.0;
        float componentMass = mass / verts.size();
        for(int i = 0; i < verts.size(); i++){
            inertiaTensor += componentMass * (verts[i].euclidianDistance(position) * verts[i].euclidianDistance(position));
        }
        inverseI = 1 / inertiaTensor;
    }
    const void finalize(){
        calculateInertiaTensor();
        boundingRect = new BoundingRect(verts);
        recalculateTriangulation();
    }
    const void recalculateTriangulation(){
        Util::triangulateNGon(verts, triangles);
    }
    
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
private:
    inline static std::size_t internalIdCount = 0;
};

#endif
