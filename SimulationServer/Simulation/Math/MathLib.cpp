#include "MathLib.h"

double cosD(double arcDegrees)
{
	return cos(M_PI * arcDegrees / 180.0);
}

double sinD(double arcDegrees)
{
	return sin(M_PI * arcDegrees / 180.0);
}

int sign(double number)
{
	return number > 0 ? 1 : -1;
}

Point orthogonalProjection(Point& p, Point& line_beg, Point& line_end, bool* belongs_to_line)
{
	Point centered = line_end - line_beg;
	double u = (p - line_beg).dot(centered) / centered.dot(centered);
	if (belongs_to_line != 0)
		*belongs_to_line = u >= 0 && u <= 1;
	double x = line_beg.getX() + line_end.getXDiff(line_beg) * u;
	double y = line_beg.getY() + line_end.getYDiff(line_beg) * u;

	return Point(x, y);
}
