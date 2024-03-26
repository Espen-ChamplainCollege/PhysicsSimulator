#ifndef BOUNDING_RECT_H
#define BOUNDING_RECT_H
#include "../../bin/point.h"

struct BoundingRect {
    Point center, min, max;
    float rx, ry;
    bool colliding;
    BoundingRect(const std::vector<Point> &vs){
        update(vs);
    }
    const void update(const std::vector<Point> &vs){
        float minx = vs[0].x;
        float miny = vs[0].y;
        float maxx = vs[0].x;
        float maxy = vs[0].y;
        for(int i = 0; i < vs.size(); i++){
            if(vs[i].x < minx) minx = vs[i].x;
            if(vs[i].x > maxx) maxx = vs[i].x;
            if(vs[i].y < miny) miny = vs[i].y;
            if(vs[i].y > maxy) maxy = vs[i].y;
        }
        center = Point((minx + maxx) / 2, (miny + maxy) / 2);
        rx = (maxx - minx) / 2;
        ry = (maxy - miny) / 2;
        min = Point(minx, miny);
        max = Point(maxx, maxy);
        grow(scalar);
    }
    const int intersectionTest(BoundingRect &o) const {
        if (std::abs(center.x - o.center.x) > (rx + o.rx)) return 0;
        if (std::abs(center.y - o.center.y) > (ry + o.ry)) return 0;
        return 1;
    }
    const float sqrtDistance(const Point &p) const {
        float sqrtDist = 0.0;
        if(p.x < min.x) sqrtDist += (min.x - p.x) * (min.x - p.x);
        if(p.x > max.x) sqrtDist += (p.x - max.x) * (p.x - max.x);
        if(p.y < min.y) sqrtDist += (min.y - p.y) * (min.y - p.y);
        if(p.y > max.y) sqrtDist += (p.y - max.y) * (p.y - max.y);
        return sqrtDist;
    }
    const void grow(const float &s){
        scalar = s;
        min = center + ((min - center) * scalar);
        max = center + ((max - center) * scalar);
        rx = (max.x - min.x) / 2;
        ry = (max.y - min.y) / 2;
    }
private:
    float scalar = 1.1;
};

#endif
