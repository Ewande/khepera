#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <stdint.h>

class Point
{
public:
	Point(double x = 0, double y = 0) : d_x(x), d_y(y) {}
	Point(const Point& other) : Point(other.d_x, other.d_y) {}
	~Point() {}
	
	double getX() { return d_x; }
	double getY() { return d_y; }

	void setCoords(double x, double y) { d_x = x; d_y = y; }
	void setCoords(Point& other) { d_x = other.getX(); d_y = other.getY(); }
	void translate(double x, double y) { d_x += x; d_y += y; }

	double getDistance(Point& other);
	double getXDiff(Point& other) { return d_x - other.getX(); }
	double getYDiff(Point& other) { return d_y - other.getY(); }
	double dot(Point& other);

	friend Point operator+(Point& fst, Point& snd);
	friend Point operator-(Point& fst, Point& snd);

private:
	double d_x;
	double d_y;

};

#endif