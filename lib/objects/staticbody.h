#ifndef STATIC_BODY_H
#define STATIC_BODY_H

#include "../../bin/point.h"
#include "../../bin/color.h"
#include "shape.h"
#include <cfloat>


struct StaticBody : Shape {
    StaticBody(
        const std::vector<Point> verts_,
        const Color &color_,
        const float &rotation_ = 0.0
    ) : Shape(Point::averageOfVector(verts_), color_, FLT_MAX, 
              Point(0, 0), rotation_, Point(0, 0), 0, 0){
        isStatic = true;
        inverseMass = 0.0;
        verts = verts_;
        finalize();
        inertiaTensor = FLT_MAX;
        inverseI = 0.0;
    }
};

#endif
