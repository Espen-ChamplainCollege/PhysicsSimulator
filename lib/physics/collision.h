#ifndef COLLISION_H
#define COLLISION_H

#include "../../bin/point.h"
#include "../../bin/util.h"
#include "../objects/shape.h"
#include <algorithm>
#include <cfloat>
#include <unordered_set>
#include <iostream>

struct CollisionKey {
    Shape* s1;
    Shape* s2;
    CollisionKey(Shape* s1_, Shape* s2_){
        s1 = (s1_->id < s2_->id) ? s1_ : s2_;
        s2 = (s1_->id < s2_->id) ? s2_ : s1_;
    }
};
inline const bool operator==(const CollisionKey &k1, const CollisionKey &k2){
    return ((*k1.s1) == (*k2.s1) && (*k1.s2) == (*k2.s2));
}
namespace std {
    template<> struct hash<CollisionKey> {
        std::size_t operator()(const CollisionKey &k) const {
            std::size_t hsh = 0;
            std::size_t h1 = std::hash<Shape*>{}(k.s1);
            std::size_t h2 = std::hash<Shape*>{}(k.s2);
            hsh ^= h1 + 0x9e3779b9 + (hsh << 6) + (hsh >> 2);
            hsh ^= h2 + 0x9e3779b9 + (hsh << 6) + (hsh >> 2);
            return hsh;
        }
    };
};

struct Feature {
    int type; // 0 = vert, 1 = edge, 2 = triangle.
    Point p1, p2, p3;
    Feature(){};
    Feature(int t, const Point &p1_, const Point &p2_ = Point(0, 0), const Point &p3_ = Point(0, 0)) : type(t), p1(p1_), p2(p2_), p3(p3_){}
    Feature(int t, const std::vector<Point> &tri) : type(t), p1(tri[0]), p2(tri[1]), p3(tri[2]){};
    Feature(const Feature &f) : type(f.type), p1(f.p1), p2(f.p2), p3(f.p3){};
    inline const bool operator==(const Feature &f) const {
        if(type != f.type) return false;
        if(type == 0) return p1 == f.p1;
        if(type == 1) return (p1 == f.p1) && (p2 == f.p2);
        if(type == 2) return (p1 == f.p1) && (p2 == f.p2) && (p3 == f.p3);
        return false;
    }
};
struct FeaturePair { 
    Feature f1, f2; 
    FeaturePair(){};
    FeaturePair(const Feature &f1_, const Feature &f2_) : f1(f1_), f2(f2_){};
    FeaturePair(const FeaturePair &o) : f1(o.f1), f2(o.f2){};
    inline const bool operator==(const FeaturePair &o) const {
        return (f1 == o.f1 && f2 == o.f2) || (f1 == o.f2 && f2 == o.f1);
    }
    std::string toString() const {
        std::string str;
        if(f1.type == 0) {
            str += "f1 [vert]: ";
            str += std::string("\n\t") + std::string(f1.p1);
        }
        if(f1.type == 1) {
            str += "f1 [edge]: ";
            str += std::string("\n\t") + std::string(f1.p1);
            str += std::string("\n\t") + std::string(f1.p2);
        }
        if(f1.type == 2) {
            str += "f1 [tri]: ";
            str += std::string("\n\t") + std::string(f1.p1);
            str += std::string("\n\t") + std::string(f1.p2);
            str += std::string("\n\t") + std::string(f1.p3);
        }
        if(f2.type == 0) {
            str += "f2 [vert]: ";
            str += std::string("\n\t") + std::string(f2.p1);
        }
        if(f2.type == 1) {
            str += "f2 [edge]: ";
            str += std::string("\n\t") + std::string(f2.p1);
            str += std::string("\n\t") + std::string(f2.p2);
        }
        if(f2.type == 2) {
            str += "f2 [tri]: ";
            str += std::string("\n\t") + std::string(f2.p1);
            str += std::string("\n\t") + std::string(f2.p2);
            str += std::string("\n\t") + std::string(f2.p3);
        }
        return str;
    }
};
struct Contact {
    Point position, normal, tangent, s1c, s2c, relv;
    float accuNormal, accuTangent, accuNormalBias; // accu as in accumulated
    float mNormal, mTangent; // m as in mass
    float bias, separation;
    FeaturePair fp;
    Shape* shape1;
    Shape* shape2;
    Contact() : position(), normal(), tangent(), s1c(), s2c(), relv(), accuNormal(0.0), accuTangent(0.0), accuNormalBias(0.0), mNormal(0.0), mTangent(0.0), bias(0.0), separation(0.0), fp(), shape1(nullptr), shape2(nullptr){}
    Contact(const Contact &o) : position(o.position), normal(o.normal), tangent(o.tangent), s1c(o.s1c), s2c(o.s2c), relv(o.relv), accuNormal(o.accuNormal), accuTangent(o.accuTangent), accuNormalBias(o.accuNormalBias), mNormal(o.mNormal), mTangent(o.mTangent), bias(o.bias), separation(o.separation), fp(o.fp), shape1(o.shape1), shape2(o.shape2){}
};

inline const static int MAX_CONTACTS = 3;
struct Collision {
    Shape* s1;
    Shape* s2;
    int contactCount;
    float friction;
    std::vector<Contact> contacts;
    bool possibleCollision;
    Collision(Shape* s1_, Shape* s2_){
        // enforce consistency
        s1 = (s1_->id < s2_->id) ? s1_ : s2_;
        s2 = (s1_->id < s2_->id) ? s2_ : s1_;
        
        contactCount = 0;
        // broad check
        possibleCollision = s1->boundingRect->intersectionTest(*s2->boundingRect);
        friction = std::sqrt(s1->friction * s2->friction);
    }
    const void refresh(const std::vector<Contact> &newContacts){
        std::vector<Contact> oldContacts = contacts;
        contacts.clear();
        for(int i = 0; i < newContacts.size(); i++){
            bool found = false;
            for(int k = 0; k < oldContacts.size(); k++){
                if(contacts.size() >= MAX_CONTACTS) continue;
                if(newContacts[i].fp == oldContacts[k].fp){
                    contacts.push_back(oldContacts[k]);
                    found = true; break;
                }
            }
            if(!found){
                contacts.push_back(newContacts[i]);
            }
        }
    }
    const float BIAS_FACTOR = 0.02;
    const float ALLOWED_PENETRATION = 0.01;
    /* Sources for the following functions:
     * https://github.com/erincatto/box2d-lite/blob/master/docs/GDC2006_Catto_Erin_PhysicsTutorial.pdf
     * https://github.com/erincatto/box2d-lite/blob/master/docs/HowDoPhysicsEnginesWork.pdf
     * https://erikonarheim.com/posts/understanding-collision-constraint-solvers/
     * https://www.geometrictools.com/Documentation/PolyhedralMassProperties.pdf
     * https://www.geometrictools.com/Documentation/ComputingImpulsiveForces.pdf
     */


    // Collision response, does not work.
    const void cache(float inverseTime){
        for(int i = 0; i < contacts.size(); i++){
            Contact* c = &contacts[i];
            Shape* b1 = c->shape1;
            Shape* b2 = c->shape2;
            Point c1 = c->position - b1->position;
            Point c2 = c->position - b2->position;

            float nd1 = Util::dot(c1, c->normal);
            float nd2 = Util::dot(c2, c->normal);
            c->mNormal = 1.0 / (b1->inverseMass + b2->inverseMass 
                + b1->inverseI * (Util::dot(c1, c1) - nd1 * nd1) 
                + b2->inverseI * (Util::dot(c2, c2) - nd2 * nd2));
            c->mNormal /= 2;

            Point tan = Util::cross2D(c->normal, 1.0);
            float td1 = Util::dot(c1, tan);
            float td2 = Util::dot(c2, tan);
            c->mTangent = 1.0 / (b1->inverseMass + b2->inverseMass
                + b1->inverseI * (Util::dot(c1, c1) - td1 * td1)
                + b2->inverseI * (Util::dot(c2, c2) - td2 * td2));
            c->mTangent /= 2;
            c->bias = -BIAS_FACTOR * inverseTime * Util::min(0.0, c->separation + ALLOWED_PENETRATION);
            Point impulse = c->normal * c->accuNormal + tan * c->accuTangent; 
            if(!b1->isStatic){
                b1->velocity -= impulse * b1->inverseMass;
                b1->angularVelocity -= b1->inverseI * Util::cross2D(c1, impulse);
            }
            if(!b2->isStatic){
                b2->velocity += impulse * b2->inverseMass;
                b2->angularVelocity += b2->inverseI * Util::cross2D(c2, impulse);
            }
        }
    }

    // Collision response, does not work.
    const void apply(){
        return;
        for(int i = 0; i < contacts.size(); i++){
            Contact* c = &contacts[i];
            Shape* b1 = c->shape1;
            Shape* b2 = c->shape2;
            Point c1 = c->position - b1->position;
            Point c2 = c->position - b2->position;
            Point relv = b2->velocity + Util::cross2D(b2->angularVelocity, c2) - b1->velocity - Util::cross2D(b1->angularVelocity, c1);

            float veln = Util::dot(relv, c->normal);
            
            float nmag = c->mNormal * (-veln + c->bias);
            float pn = c->accuNormal;
            c->accuNormal = Util::max(pn + nmag, 0.0);
            nmag = c->accuNormal - pn;

            Point normalImpulse = c->normal * nmag;
            if(!b1->isStatic){
                b1->velocity -= normalImpulse * b1->inverseMass;
                b1->angularVelocity -= b1->inverseI * Util::cross2D(c1, normalImpulse);
            }
            if(!b2->isStatic){
                b2->velocity += normalImpulse * b2->inverseMass;
                b2->angularVelocity += b2->inverseI * Util::cross2D(c2, normalImpulse);
            }

            relv = b2->velocity + Util::cross2D(b2->angularVelocity, c2) - b1->velocity - Util::cross2D(b1->angularVelocity, c1);
            Point tan = Util::cross2D(c->normal, 1.0);
            float velt = Util::dot(relv, tan);
            float tmag = c->mTangent * (-velt);
            float maxTan = friction * c->accuNormal;
            float oldTan = c->accuTangent;
            c->accuTangent = Util::clamp(oldTan + tmag, -maxTan, maxTan);
            tmag = c->accuTangent -oldTan;
            Point tangentImpulse = tan * tmag;
            if(!b1->isStatic){
                b1->velocity -= tangentImpulse * b1->inverseMass;
                b1->angularVelocity -= b1->inverseI * Util::cross2D(c1, tangentImpulse);
            }
            if(!b2->isStatic){
                b2->velocity += tangentImpulse * b2->inverseMass;
                b2->angularVelocity += b2->inverseI * Util::cross2D(c2, tangentImpulse);
            }

        }
    }
    const float CONTACT_THRESHOLD = 1;
    const void edgeTest(Shape* shape1, Shape* shape2, const std::vector<Point> &verts, const std::vector<m_Edge> &edges){
        for(int i = 0; i < verts.size(); i++){
            for(int k = 0; k < edges.size(); k++){
                if(contacts.size() >= MAX_CONTACTS) return;
                float d = Util::sqDistanceToLine(edges[k].a, edges[k].b, verts[i]);
                if(d < CONTACT_THRESHOLD){
                    Contact c;
                    c.shape1 = shape1;
                    c.shape2 = shape2;
                    c.position = shape1->verts[i];
                    c.normal = -Util::getRotated90Left(edges[k].b - edges[k].a);
                    c.separation = -Util::magnitude(c.normal);
                    Util::normalize(c.normal);
                    c.fp = FeaturePair(
                        Feature(0, Util::getRotatedPoint(Point(0, 0), -shape1->rotation, verts[i])),
                        Feature(2, Util::getRotatedPoints(shape2->position - shape1->position, -shape2->rotation, {edges[k].a, edges[k].b}))
                    );
                    contacts.push_back(c);
                }
            }
        }
    }
    std::vector<Point> dClosest;
    std::vector<m_Edge> dSplitEdges;
    std::vector<m_Edge> dTestEdges;
    std::vector<Point> intersections;
    const void triangleTest(Shape* shape1, Shape* shape2, const std::vector<Point> &verts, const std::vector<std::vector<Point>> &triangles){
        for(int i = 0; i < verts.size(); i++){
            for(int k = 0; k < triangles.size(); k++){
                if(contacts.size() >= MAX_CONTACTS) return;
                if(Util::pointInTriangle(verts[i], triangles[k])){
                    Contact c;
                    c.shape1 = shape1;
                    c.shape2 = shape2;
                    c.position = shape1->verts[i];
                    std::pair<m_Edge, m_Edge> edges = Util::getPointsEdges(verts, verts[i]);
                    dSplitEdges.push_back(edges.first);
                    dSplitEdges.push_back(edges.second);
                    m_Edge contactEdge;
                    std::vector<Point> s2v = shape2->verts;
                    std::transform(s2v.begin(), s2v.end(), s2v.begin(), [&](Point &p){ return p - shape1->position;});
                    std::vector<m_Edge> shapeEdges = Util::breakIntoEdges(s2v);
                    bool doubleIntersection = false;
                    for(int q = 0; q < shapeEdges.size(); q++){
                        if(doubleIntersection){
                            if(Util::edgeIntersectEdge(edges.first, shapeEdges[q], intersections) && Util::edgeIntersectEdge(edges.second, shapeEdges[q], intersections)){
                                contactEdge = shapeEdges[q];
                                doubleIntersection = true;
                            }
                        } else {
                            if(Util::edgeIntersectEdge(edges.first, shapeEdges[q], intersections) || Util::edgeIntersectEdge(edges.second, shapeEdges[q], intersections)){
                                contactEdge = shapeEdges[q];
                                doubleIntersection = true;
                            }
                        }  
                    }
                    
                    dTestEdges.push_back(contactEdge);

                    
                    Point closest = Util::closestPointOnEdge(verts[i], contactEdge);

                    dClosest.push_back(closest);
                    c.normal = -(closest - verts[i]);
                    c.separation = -Util::magnitude(c.normal);
                    Util::normalize(c.normal);
                    c.fp = FeaturePair(
                        Feature(0, Util::getRotatedPoint(Point(0, 0), -shape1->rotation, verts[i])),
                        Feature(2, Util::getRotatedPoints(shape2->position - shape1->position, -shape2->rotation, triangles[k]))
                    );
                    contacts.push_back(c);
                }
            }
        }
    }
    const int realIntersectionTest(){
        dClosest.clear();
        intersections.clear();
        dTestEdges.clear();
        dSplitEdges.clear();
        Shape* shape1 = (s1->triangles.size() >= s2->triangles.size()) ? s1 : s2;
        Shape* shape2 = (shape1 == s1) ? s2 : s1;
        std::unordered_set<Point> addedPoints;
        std::vector<Point> verts = shape1->verts;
        std::transform(verts.begin(), verts.end(), verts.begin(), [&](Point &p){ return p - shape1->position; });
        std::vector<std::vector<Point>> triangles = shape2->triangles;
        std::transform(triangles.begin(), triangles.end(), triangles.begin(), [&](std::vector<Point> &t){ return std::vector<Point>{t[0] - shape1->position, t[1] - shape1->position, t[2] - shape1->position}; });
        std::vector<m_Edge> edges = Util::breakIntoEdges(triangles);

        edgeTest(shape1, shape2, verts, edges);
        if(contacts.size() >= MAX_CONTACTS) return contacts.size();
        triangleTest(shape1, shape2, verts, triangles);
        if(contacts.size() >= MAX_CONTACTS) return contacts.size();

        shape1 = shape1 == s1 ? s2 : s1;
        shape2 = shape1 == s1 ? s2 : s1;
        verts = shape1->verts;
        std::transform(verts.begin(), verts.end(), verts.begin(), [&](Point &p){ return p - shape1->position; });
        triangles = shape2->triangles;
        std::transform(triangles.begin(), triangles.end(), triangles.begin(), [&](std::vector<Point> &t){ return std::vector<Point>{t[0] - shape1->position, t[1] - shape1->position, t[2] - shape1->position}; });
        edges = Util::breakIntoEdges(triangles);
        edgeTest(shape1, shape2, verts, edges);
        if(contacts.size() >= MAX_CONTACTS) return contacts.size();
        triangleTest(shape1, shape2, verts, triangles);
        return contacts.size();
    }

};

#endif
