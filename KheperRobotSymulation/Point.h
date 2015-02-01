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
	
	double GetX() { return d_x; }
	double GetY() { return d_y; }

	void SetCords(double x, double y) { d_x = x; d_y = y; }
	void SetCords(Point& other) { d_x = other.GetX(); d_y = other.GetY(); }
	void Translate(double x, double y) { d_x += x; d_y += y; }

	double GetDistance(Point& other);
	double GetXDiff(Point& other) { return d_x - other.GetX(); }
	double GetYDiff(Point& other) { return d_y - other.GetY(); }
	double Dot(Point& other);

	friend Point operator+(Point& fst, Point& snd);
	friend Point operator-(Point& fst, Point& snd);

private:
	double d_x;
	double d_y;

};

#endif