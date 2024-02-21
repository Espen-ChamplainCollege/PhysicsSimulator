#ifndef HEXAGON_H
#define HEXAGON_H


#include "point.h"
#include "color.h"
#include "shape.h"
#include "util.h"

struct Hexagon : Shape {
	float width;
  Hexagon(
    const Point vertices[6],
    const Color &color_,
    const Point &initialVelocity = Point(0, 0),
    const float &initialRotation = 0,
    const Point &initialForce = Point(0, 0),
    const float &initialAngularVelocity = 0,
    const float &initialTorque = 0
  ) : Shape(Point::averageOfArray(vertices, 6), 
            color_, initialVelocity, initialRotation, initialForce,
            initialAngularVelocity, initialTorque){

    for(int i = 0; i < 6; i++){
      verts.push_back(vertices[i]);
    }
  }
  Hexagon(
    const Point &center,
    const float width_,
    const Color &color_,
    const Point &initialVelocity = Point(0, 0),
    const float &initialRotation = 0,
    const Point &initialForce = Point(0, 0),
    const float &initialAngularVelocity = 0,
    const float &initialTorque = 0
  ) : width(width_), Shape(center, 
            color_, initialVelocity, initialRotation, initialForce,
            initialAngularVelocity, initialTorque){
    verts = Util::constructHexagon(center, width);
  }
};


#endif
