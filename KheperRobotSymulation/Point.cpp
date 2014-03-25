#include "Point.h"

double Point::GetDistance(Point& other)
{
	double x_diff = d_x - other.dGetX();
	double y_diff = d_y - other.dGetY();
	return sqrt(x_diff * x_diff + y_diff * y_diff);

}