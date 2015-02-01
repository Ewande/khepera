#include "Point.h"

double Point::GetDistance(Point& other)
{
	double x_diff = GetXDiff(other);
	double y_diff = GetYDiff(other);
	return sqrt(x_diff * x_diff + y_diff * y_diff);
}

double Point::Dot(Point& other)
{
	return d_x * other.GetX() + d_y * other.GetY();
}

Point operator+(Point& fst, Point& snd)
{
	return Point(fst.d_x + snd.d_x, fst.d_y + snd.d_y);
}
Point operator-(Point& fst, Point& snd)
{
	return Point(fst.d_x - snd.d_x, fst.d_y - snd.d_y);
}