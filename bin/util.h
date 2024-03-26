#ifndef UTIL_H
#define UTIL_H
#include "point.h"
#include <algorithm>
#include <cmath>
#include "qpoint.h"
#include <unordered_set>

struct m_Edge {
    Point a, b;
    m_Edge(const Point &a_, const Point &b_){
        a = a_ < b_ ? a_ : b_;
        b = a == a_ ? b_ : a_;
    };
    m_Edge(){};
    inline const bool operator==(const m_Edge &o) const {
        return (a == o.a && b == o.b);
    }
    inline const m_Edge operator+(const Point &p) const {
        return m_Edge(a + p, b + p);
    }
};
namespace std {
    template<> struct hash<m_Edge> {
        std::size_t operator()(const m_Edge &e) const {
            std::size_t hsh = 0;
            std::size_t h1 = std::hash<Point>{}(e.a);
            std::size_t h2 = std::hash<Point>{}(e.b);
            hsh ^= h1 + 0x9e3779b9 + (hsh << 6) + (hsh >> 2);
            hsh ^= h2 + 0x9e3779b9 + (hsh << 6) + (hsh >> 2);
            return hsh;
        }
    };
};
namespace Util {
const static std::vector<m_Edge> breakIntoEdges(const std::vector<std::vector<Point>> &triangles){
    std::unordered_set<m_Edge> edges;
    for(int i = 0; i < triangles.size(); i++){
        for(int k = 0; k < triangles[i].size(); k++){
            if(edges.find(m_Edge(triangles[i][k], triangles[i][(k + 1) % 3])) == edges.end()){
                edges.insert(m_Edge(triangles[i][k], triangles[i][(k + 1) % 3]));
            }
        }
    }
    return std::vector<m_Edge>(edges.begin(), edges.end());
}
const static std::vector<m_Edge> breakIntoEdges(const std::vector<Point> &verts){
    std::vector<m_Edge> edges;
    for(int i = 0; i < verts.size(); i++){
        edges.push_back(m_Edge(verts[i], verts[(i + 1) % verts.size()]));
    }
    return edges;
}

const static std::vector<Point> constructHexagon(const Point &center, const float width){
    std::vector<Point> points;
    points.push_back(Point(center.x - width, center.y));
    points.push_back(Point(center.x - width / 2, center.y - width * 0.875));
    points.push_back(Point(center.x + width / 2, center.y - width * 0.875));
    points.push_back(Point(center.x + width, center.y));
    points.push_back(Point(center.x + width / 2, center.y + width * 0.875));
    points.push_back(Point(center.x - width / 2, center.y + width * 0.875));
    return points;
}
const static std::vector<Point> constructSquare(const Point &center, const float width){
    std::vector<Point> points(4);
    points[0] = Point(center.x + width, center.y - width);
    points[1] = Point(center.x - width, center.y - width);
    points[2] = Point(center.x - width, center.y + width);
    points[3] = Point(center.x + width, center.y + width);
    return points;
}
const static std::vector<Point> constructTriangle(const Point &center, const float length){
    std::vector<Point> points;
    points.push_back(Point(center.x, center.y + length));
    points.push_back(Point(center.x - length - std::sqrt(3) / 2, center.y - length / 2));
    points.push_back(Point(center.x + length + std::sqrt(3) / 2, center.y - length / 2));
    return points;
}
const static double generalRandom(const double min, const double max){
    std::random_device device;
    std::mt19937 mt(device());
    std::uniform_real_distribution<> dist(min, max);
    return dist(mt);
}
const static QPointF* convertPointVector(const std::vector<Point> &vect){
    QPointF *array = (QPointF*)malloc(vect.size() * sizeof(QPointF));
    std::transform(vect.begin(), vect.end(), array, [](const Point &p){ return QPointF(p.x, p.y); });
    return array;
}
const static double PI = 3.14159265358979323846;
const inline static float degreeToRadian(const float &degrees){ return (degrees * PI) / 180; }
const inline static float radianToDegree(const float &radian){ return (radian * 180) / PI; }
const static void rotatePoints(const Point &center, float rotation, std::vector<Point> &points){
    rotation = degreeToRadian(rotation);
    float sine = std::sin(rotation);
    float cosine = std::cos(rotation);
    for(int i = 0; i < points.size(); i++){
        double xtemp = (cosine * (points[i].x - center.x) - sine * (points[i].y - center.y)) + center.x;
        points[i].y = (sine * (points[i].x - center.x) + cosine * (points[i].y - center.y)) + center.y;
        points[i].x = xtemp;
    }
}
const static Point getRotatedPoint(const Point &center, float rotation, const Point &point){
    rotation = degreeToRadian(rotation);
    float sine = std::sin(rotation);
    float cosine = std::cos(rotation);
    Point result;
    result.x = (cosine * (point.x - center.x) - sine * (point.y - center.y)) + center.x;
    result.y = (sine * (point.x - center.x) + cosine * (point.y - center.y)) + center.y;
    return result;
}
const static std::vector<Point> getRotatedPoints(const Point &center, float rotation, const std::vector<Point> &points){
    rotation = degreeToRadian(rotation);
    float sine = std::sin(rotation);
    float cosine = std::cos(rotation);
    std::vector<Point> result(points.size());
    for(int i = 0; i < points.size(); i++){
        result[i].x = (cosine * (points[i].x - center.x) - sine * (points[i].y - center.y)) + center.x;
        result[i].y = (sine * (points[i].x - center.x) + cosine * (points[i].y - center.y)) + center.y;
    }
    return result;
}


const static void realignPoints(std::vector<Point> &points, const Point &translation){
  for(int i = 0; i < points.size(); i++) points[i] += translation;
}
const inline static void rotate90Right(Point &v){
    float temp = v.x;
    v.x = -v.y; v.y = temp;
}
const inline static void rotate90Left(Point &v){
    float temp = v.x;
    v.x = v.y; v.y = -temp;
}
const inline static Point getRotated90Right(const Point &v){
    return Point(-v.y, v.x);
}
const inline static Point getRotated90Left(const Point &v){
    return Point(v.y, -v.x);
}
const inline static float magnitude(const Point &v){
    return std::sqrt(v.x * v.x + v.y * v.y);
}
const inline static Point tripleProduct(const Point &a, const Point &b, const Point &c){
    float prod = a.x * b.y - b.x * a.y;
    return Point(-c.y * prod, c.x * prod);
}
const static float dot(const Point &p1, const Point &p2){
    return p1.x * p2.x + p1.y * p2.y;
}
inline const static float cross2D(const Point &p1, const Point &p2){
    return p1.y * p2.x - p1.x * p2.y;
}
inline const static Point cross2D(const Point &p1, const float &v){
    return Point(p1.y * v, p1.x * (-v));
}
inline const static Point cross2D(const float &v, const Point &p1){
    return Point(-v * p1.y, v * p1.x);
}
inline const static float max(float v1, float v2){
    return v1 > v2 ? v1 : v2;
}
inline const static float min(float v1, float v2){
    return v1 < v2 ? v1 : v2;
}
inline const float clamp(float v, float minv, float maxv){
    return max(minv, min(v, maxv));
}
inline const static Point getPerpendicular(const Point &p){
    return Point(-p.y == 0.0 ? 0.0 : -p.y, p.x);
}
inline const static bool sameSign(float a, float b){
    return (a * b) > 0.0;
}
const static std::pair<m_Edge, m_Edge> getPointsEdges(const std::vector<Point> &verts, const Point &p){
    for(int i = 0; i < verts.size(); i++){
        if(verts[i] == p){
            return {m_Edge(verts[(i - 1) % verts.size()], p), m_Edge(verts[(i + 1) % verts.size()], p)};
        }
    }
    return {m_Edge(p, p), m_Edge(p, p)};
}
const static int getFurthestPoint(const std::vector<Point> &verts, const Point &p){
    int index = 0;
    float maxdot = Util::dot(p, verts[0]);
    for(int i = 1; i < verts.size(); i++){
        float thisdot = Util::dot(p, verts[i]);
        if(thisdot > maxdot){
            maxdot = thisdot;
            index = i;
        }
    }
    return index;
}
const static void normalize(Point &v){
    float mag = std::sqrt(dot(v, v));
    v.x /= mag; v.y /= mag;
}
const static bool triangleCCW(const Point &a, const Point &b, const Point &c){
    return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}
const static bool pointInTriangle(const Point &p, const Point &a, const Point &b, const Point &c){
    float pab = Util::cross2D(p - a, b - a);
    float pbc = Util::cross2D(p - b, c - b);
    if(!Util::sameSign(pab, pbc)) return false;
    float pca = Util::cross2D(p - c, a - c);
    if(!Util::sameSign(pab, pca)) return false;
    return true;
}
inline const static bool pointInTriangle(const Point &p, const std::vector<Point> &t){
    return pointInTriangle(p, t[0], t[1], t[2]);
}
inline const static float signedTriangleArea(const Point &a, const Point &b, const Point &c){
    return (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
}
const static float sqDistanceToLine(const Point &a, const Point &b, const Point &p){
    Point ab = b - a;
    Point ap = p - a;
    Point bp = p - b;
    float e = dot(ap, ab);
    if(e <= 0.0) return dot(ap, ap);
    float f = dot(ab, ab);
    if(e >= f) return dot(bp, bp);
    return dot(ap, ap) - e * e / f;
}
const static float sqDistanceToLine(const m_Edge &e, const Point &p){
    return sqDistanceToLine(e.a, e.b, p);
}

const static bool lineIntersectLine(const Point &a1, const Point &a2, const Point &b1, const Point &b2, Point &intersection){
    float area1 = signedTriangleArea(a1, a2, b2);
    float area2 = signedTriangleArea(a1, a2, b1);
    if(!sameSign(area1, area2)){
        float area3 = signedTriangleArea(b1, b2, a1);
        float area4 = area3 + area2 - area1;
        if(!sameSign(area3, area4)){
            float t = area3 / (area3 - area4);
            intersection = a1 + (a2 - a1) * t;
            return true;
        }
    }
    return false;
}
inline const static bool edgeIntersectEdge(const m_Edge &e1, const m_Edge &e2, Point &intersection){
    return lineIntersectLine(e1.a, e1.b, e2.a, e2.b, intersection);
}
inline const static bool edgeIntersectEdge(const m_Edge &e1, const m_Edge &e2, std::vector<Point> &intersections){
    Point intersection;
    if(lineIntersectLine(e1.a, e1.b, e2.a, e2.b, intersection)){
        intersections.push_back(intersection);
        return true;
    }
    return false;
}
const static m_Edge closestEdgeToPoint(const std::vector<m_Edge> &edges, const Point &point){
    float cd = sqDistanceToLine(edges[0], point);
    int index = 0;
    for(int i = 1; i < edges.size(); i++){
        float d = sqDistanceToLine(edges[i], point);
        if(d < cd){
            cd = d;
            index = i;
        }
    }
    return edges[index];
}
const static Point closestPointOnEdge(const Point &p, const m_Edge &e){
    float t = dot(p - e.a, e.b - e.a) / dot(e.b - e.a, e.b - e.a);
    t = clamp(t, 0.0, 1.0);
    return Point(e.a + (e.b - e.a) * t);
}
// http://www.cs.unc.edu/~dm/CODE/GEM/chapter.html
// https://sites.cs.ucsb.edu/~suri/cs235/Triangulation.pdf
// Christer Ericson - Real-Time Collision Detection Chapter 12.5
const static void triangulateNGon(const std::vector<Point> &vs, std::vector<std::vector<Point>> &triangles){
    triangles.clear();
    int n = vs.size();
    int* prev = new int[n];
    int* next = new int[n];
    for(int i = 0; i < n; i++){
        prev[i] = i - 1;
        next[i] = i + 1;
    }
    prev[0] = n - 1;
    next[n - 1] = 0;

    int i = 0;
    while(n >= 3){
        bool isEar = true;
        if(triangleCCW(vs[prev[i]], vs[i], vs[next[i]])){
            int k = next[next[i]];
            do{
                if(pointInTriangle(vs[k], vs[prev[i]], vs[i], vs[next[i]])){
                    isEar = false;
                    break;
                }
                k = next[k];
            } while(k != prev[i]);
        } else {
            isEar = false;
        }
        if(isEar){
            triangles.push_back({vs[i], vs[prev[i]], vs[next[i]]});
            next[prev[i]] = next[i];
            prev[next[i]] = prev[i];
            n--;
            i = prev[i];
        } else {
            i = next[i];
        }
    }
}

};
#endif
