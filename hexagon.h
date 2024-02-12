#ifndef HEXAGON_H
#define HEXAGON_H


#include "point.h"
#include "color.h"

struct Hexagon {
	Point position;
	Point velocity;
	// painter.drawPolygon() requires a QPointF
    QPointF points[6];
	float radius;
	Color color;
	//https://doc.qt.io/qt-6/qpainter.html#drawConvexPolygon
	//https://www.quora.com/How-can-you-find-the-coordinates-in-a-hexagon 
	Hexagon(const Point& p, const Color& c, float r)
		: position(p), color(c), radius(r) 
	{
		// 6 vertices are placed in relation to center 'position' point
		points[0] = QPointF(position.x - 16, position.y);
		points[1] = QPointF(position.x - 8, position.y - 14);
		points[2] = QPointF(position.x + 8, position.y - 14);
		points[3] = QPointF(position.x + 16, position.y);
		points[4] = QPointF(position.x + 8, position.y + 14);
		points[5] = QPointF(position.x - 8, position.y + 14);
	};
	Hexagon(const Point& p, const Point& v, const Color& c, float r)
		: position(p), velocity(v), color(c), radius(r) 
	{
		points[0] = QPointF(position.x - 16, position.y);
		points[1] = QPointF(position.x - 8, position.y - 14);
		points[2] = QPointF(position.x + 8, position.y - 14);
		points[3] = QPointF(position.x + 16, position.y);
		points[4] = QPointF(position.x + 8, position.y + 14);
		points[5] = QPointF(position.x - 8, position.y + 14);
	};
	/*
	#include "hexagon.h"
	 
	Hexagon hexagon

	hexagon = new Hexagon(Point(50, 50), Point(2.5, 2.5), Color(255, 255, 255), 25);

	painter.drawPolygon(hexagon->points, 6);
	
	for (int i = 0; i < 6; i++)
    {
      sphere->points[i].setX(sphere->points[i].x() + sphere->velocity.x);
      sphere->points[i].setY(sphere->points[i].y() + sphere->velocity.y);
    }
	*/
};


#endif