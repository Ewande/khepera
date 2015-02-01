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
	double u = (p - line_beg).Dot(centered) / centered.Dot(centered);
	if (belongs_to_line != 0)
		*belongs_to_line = u >= 0 && u <= 1;
	double x = line_beg.GetX() + line_end.GetXDiff(line_beg) * u;
	double y = line_beg.GetY() + line_end.GetYDiff(line_beg) * u;

	return Point(x, y);
}
