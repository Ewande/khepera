#include "Point.h"

double Point::GetDistance(Point& other)
{
	double x_diff = d_x - other.GetX();
	double y_diff = d_y - other.GetY();
	return sqrt(x_diff * x_diff + y_diff * y_diff);

}