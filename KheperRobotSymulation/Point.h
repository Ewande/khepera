#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <stdint.h>

class Point
{
public:
	Point(double x = 0, double y = 0) : d_x(x), d_y(y) {}
	~Point() {}
	
	double dGetX() { return d_x; }
	double dGetY() { return d_y; }
	
	uint32_t iGetX() { return (uint32_t)d_x; }
	uint32_t iGetY() { return (uint32_t)d_y; }

	void SetCords(double x, double y) { d_x = x; d_y = y; }
	void SetCords(Point& other) { d_x = other.dGetX(); d_y = other.dGetY(); }
	void Translate(double x, double y) { d_x += x; d_y += y; }

	double GetDistance(Point& other);
	double GetXDiff(Point& other) { return d_x - other.dGetX(); }
	double GetYDiff(Point& other) { return d_y - other.dGetY(); }

private:
	double d_x;
	double d_y;

};

#endif