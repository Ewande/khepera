#include "Point.h"

double Point::getDistance(Point& other)
{
	double x_diff = getXDiff(other);
	double y_diff = getYDiff(other);
	return sqrt(x_diff * x_diff + y_diff * y_diff);
}

double Point::dot(Point& other)
{
	return _x * other.getX() + _y * other.getY();
}

bool Point::isBetween(Point& first, Point& second)
{
    return getDistance(first) + getDistance(second) - first.getDistance(second) <= EPS;
}

Point operator+(Point& fst, Point& snd)
{
	return Point(fst._x + snd._x, fst._y + snd._y);
}
Point operator-(Point& fst, Point& snd)
{
	return Point(fst._x - snd._x, fst._y - snd._y);
}