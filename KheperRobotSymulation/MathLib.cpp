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