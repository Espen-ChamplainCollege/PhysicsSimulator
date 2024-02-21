#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <ostream>
#include <random>

#ifdef QGLOBAL_H
#include <qpoint.h>
#endif

struct Point {
  double x, y;
  Point(double x_, double y_) : x(x_), y(y_){};
  Point(const Point &p) : x(p.x), y(p.y){};
  Point(const std::pair<double, double> &p) : x(p.first), y(p.second){};
  #ifdef QGLOBAL_H
  Point(const QPoint &p) : x(p.x()), y(p.y()){};
  Point(const QPointF &p) : x(p.x()), y(p.y()){};
  #endif
  Point() : x(0), y(0){};
  // distance functions
  const double euclidianDistance(const Point &o) const { return std::sqrt(std::pow(o.x - x, 2) + std::pow(o.y - y, 2)); }
  const int taxicabDistance(const Point &o) const { return std::abs(x - o.x) + std::abs(y - o.y); }
  const int chebyshevDistance(const Point &o) const { return std::max(std::abs(o.x - x), std::abs(o.y - y)); }
  const static double euclidianDistance(const Point &p1, const Point &p2) { return p1.euclidianDistance(p2); }
  const static int taxicabDistance(const Point &p1, const Point &p2) { return p1.taxicabDistance(p2); }
  const static int chebyshevDistance(const Point &p1, const Point &p2) { return p1.chebyshevDistance(p2); }
  const static Point averageOfArray(const Point points[], int count){
    Point sum = Point(0, 0);
    for(int i = 0; i < count; i++){
      sum += points[i];
    }
    return sum / count;
  }
  // is valid, checks in point is in bounds
  const bool isValid(double minX, double maxX, double minY, double maxY) const { return x >= minX && x <= maxX && y >= minY && y <= maxY ? true : false; }
  // returns where the point would be if it was flattened to 1D.
  // or: if each row was put side to side in the 2D area, where would the point be?
  const int toSingle(int div) const { return y * div + x; }
  // reverse of toSingle
  const static Point fromSingle(int n, int div) { return Point(n % div, std::floor(n / div)); }
  // random point in range
  const static Point randomPoint(double minX, double maxX, double minY, double maxY){
    std::random_device device;
    std::mt19937 mt(device());
    std::uniform_real_distribution<> distX(minX, maxX);
    std::uniform_real_distribution<> distY(minY, maxY);
    return Point(distX(mt), distY(mt));
  }
  // to pair
  const std::pair<double, double> toPair() const { return {x, y}; }
  const static std::pair<double, double> toPair(const Point &p) { return {p.x, p.y}; }
  // operators
  const bool operator==(const Point &o) const { return o.x == x && o.y == y ? true : false; }
  const bool operator!=(const Point &o) const { return o.x == x && o.y == y ? false : true; }
  const bool operator>(const Point &o) const { return ((y > o.y) || (y == o.y && x > o.x)) ? true : false; }
  const bool operator<(const Point &o) const { return ((y < o.y) || (y == o.y && x < o.x)) ? true : false; }
  const Point operator+(const double &v) const { return Point(x + v, y + v); }
  const Point operator-(const double &v) const { return Point(x - v, y - v); }
  const Point operator*(const double &v) const { return Point(x * v, y * v); }
  const Point operator/(const double &v) const { return Point(x / v, y / v); }
  const void operator+=(const double &v) { x += v; y += v; }
  const void operator-=(const double &v) { x -= v; y -= v; }
  const void operator*=(const double &v) { x *= v; y *= v; }
  const void operator/=(const double &v) { x /= v; y /= v; }
  const Point operator+(const Point &o) const { return Point(x + o.x, y + o.y); }
  const Point operator-(const Point &o) const { return Point(x - o.x, y - o.y); }
  const Point operator*(const Point &o) const { return Point(x * o.x, y * o.y); }
  const Point operator/(const Point &o) const { return Point(x / o.x, y / o.y); }
  const void operator+=(const Point &o) { x += o.x; y += o.y; }
  const void operator-=(const Point &o) { x -= o.x; y -= o.y; }
  const void operator*=(const Point &o) { x *= o.x; y *= o.y; }
  const void operator/=(const Point &o) { x /= o.x; y /= o.y; }
  explicit operator const std::string() const { return std::string('(' + std::to_string(x) + ", " + std::to_string(y) + ')'); }
  
  #ifdef QGLOBAL_H
  explicit operator const QPoint() const { return QPoint(x, y); }
  explicit operator const QPointF() const {return QPointF(x, y); }
  #endif
};
// so the point can be printed to streams
static std::ostream &operator<<(std::ostream &os, const Point &p) { os << '(' << p.x << ", " << p.y << ')'; return os; }

// so the point can be used in maps, sets, anything that needs hashing
namespace std {
template<> struct hash<Point> {
  std::size_t operator()(const Point &p) const {
    std::size_t hsh = 0;
    std::size_t h1 = std::hash<double>{}(p.x);
    std::size_t h2 = std::hash<double>{}(p.y);
    hsh ^= h1 + 0x9e3779b9 + (hsh << 6) + (hsh >> 2);
    hsh ^= h2 + 0x9e3779b9 + (hsh << 6) + (hsh >> 2);
    return hsh;
  }
};
};

#endif
