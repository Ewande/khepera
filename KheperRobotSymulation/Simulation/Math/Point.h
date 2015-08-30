#ifndef POINT_H
#define POINT_H

#include <cmath>

class Point
{
public:
	Point(double x = 0, double y = 0) : _x(x), _y(y) {}
	
	double getX() { return _x; }
	double getY() { return _y; }

	void setCoords(double x, double y) { _x = x; _y = y; }
	void setCoords(Point& other) { _x = other.getX(); _y = other.getY(); }
	void translate(double x, double y) { _x += x; _y += y; }

	double getDistance(Point& other);
	double getXDiff(Point& other) { return _x - other.getX(); }
	double getYDiff(Point& other) { return _y - other.getY(); }
	double dot(Point& other);

	friend Point operator+(Point& fst, Point& snd);
	friend Point operator-(Point& fst, Point& snd);

private:
	double _x;
	double _y;

};

#endif