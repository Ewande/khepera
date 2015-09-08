#ifndef POINT_H
#define POINT_H

#include <cmath>
#include "../Constants.h"

class Point
{
public:
	Point(double x = 0, double y = 0) : _x(x), _y(y) {}
	
	double getX() const { return _x; }
	double getY() const { return _y; }

	void setCoords(double x, double y) { _x = x; _y = y; }
	void setCoords(Point& other) { _x = other.getX(); _y = other.getY(); }
	void translate(double x, double y) { _x += x; _y += y; }

	double getDistance(Point& other) const;
	double getXDiff(Point& other) const { return _x - other.getX(); }
	double getYDiff(Point& other) const { return _y - other.getY(); }
	double dot(Point& other);
    double cross(Point& other); // returns z-coord of resultative vector (x and y are 0 when we consider 2D points)
    bool isBetween(Point& first, Point& second);

	friend Point operator+(Point& fst, Point& snd);
	friend Point operator-(Point& fst, Point& snd);

private:
	double _x;
	double _y;

};

#endif