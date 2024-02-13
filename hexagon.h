#ifndef HEXAGON_H
#define HEXAGON_H


#include "point.h"
#include "color.h"

struct Hexagon {
	Point position;
	Point velocity;
	// painter.drawPolygon() requires a QPointF
	QPointF points[6];
	//float radius;
	Color color;
	Hexagon(const Point& p, const Color& c)
		: position(p), color(c)
	{
		// 6 vertices are placed in relation to center 'position' point
		// How I decided upon coordinates for points:
		// https://www.quora.com/How-can-you-find-the-coordinates-in-a-hexagon 
		// Would probably be best to implement a multiplier in the future
		// for different sizes
		points[0] = QPointF(position.x - 16, position.y);
		points[1] = QPointF(position.x - 8, position.y - 14);
		points[2] = QPointF(position.x + 8, position.y - 14);
		points[3] = QPointF(position.x + 16, position.y);
		points[4] = QPointF(position.x + 8, position.y + 14);
		points[5] = QPointF(position.x - 8, position.y + 14);
	};
	Hexagon(const Point& p, const Point& v, const Color& c)
		: position(p), velocity(v), color(c)
	{
		points[0] = QPointF(position.x - 16, position.y);
		points[1] = QPointF(position.x - 8, position.y - 14);
		points[2] = QPointF(position.x + 8, position.y - 14);
		points[3] = QPointF(position.x + 16, position.y);
		points[4] = QPointF(position.x + 8, position.y + 14);
		points[5] = QPointF(position.x - 8, position.y + 14);
	};
};


#endif