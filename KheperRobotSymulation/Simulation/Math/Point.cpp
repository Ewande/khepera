#include "Point.h"

double Point::getDistance(Point& other)
{
	double x_diff = getXDiff(other);
	double y_diff = getYDiff(other);
	return sqrt(x_diff * x_diff + y_diff * y_diff);
}

double Point::dot(Point& other)
{
	return d_x * other.getX() + d_y * other.getY();
}

Point operator+(Point& fst, Point& snd)
{
	return Point(fst.d_x + snd.d_x, fst.d_y + snd.d_y);
}
Point operator-(Point& fst, Point& snd)
{
	return Point(fst.d_x - snd.d_x, fst.d_y - snd.d_y);
}