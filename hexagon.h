#ifndef HEXAGON_H
#define HEXAGON_H


#include "point.h"
#include "color.h"

struct Hexagon {
	Point position;
	Point velocity;
	// painter.drawPolygon() requires a QPointF
	QPointF points[6];
  float rotation;
  float angularVelocity;
	float width;
	Color color;
	Hexagon(const Point& p, const Color& c, float w)
		: position(p), color(c), width(w)
	{
		// 6 vertices are placed in relation to center 'position' point
		points[0] = QPointF(position.x - w, position.y);
		points[1] = QPointF(position.x - w / 2, position.y - w * 0.875);
		points[2] = QPointF(position.x + w / 2, position.y - w * 0.875);
		points[3] = QPointF(position.x + w, position.y);
		points[4] = QPointF(position.x + w / 2, position.y + w * 0.875);
		points[5] = QPointF(position.x - w / 2, position.y + w * 0.875);

	};
	Hexagon(const Point& p, const Point& v, const Color& c, float w)
		: position(p), velocity(v), color(c), width(w)
	{
		points[0] = QPointF(position.x - w, position.y);
		points[1] = QPointF(position.x - w / 2, position.y - w * 0.875);
		points[2] = QPointF(position.x + w / 2, position.y - w * 0.875);
		points[3] = QPointF(position.x + w, position.y);
		points[4] = QPointF(position.x + w / 2, position.y + w * 0.875);
		points[5] = QPointF(position.x - w / 2, position.y + w * 0.875);

	};
};


#endif
