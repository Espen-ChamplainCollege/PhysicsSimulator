#ifndef UTIL_H
#define UTIL_H
#include "shape.h"
#include "point.h"
#include <algorithm>
#include <cmath>
#include "qpoint.h"

namespace Util {


const static double distanceFromCenter(const Shape &s1, const Shape &s2){
  return std::sqrt(
    std::pow(s1.position.x - s2.position.x, 2) + 
    std::pow(s1.position.y - s2.position.y, 2)
  );
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



};
#endif
