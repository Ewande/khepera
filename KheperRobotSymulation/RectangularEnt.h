#ifndef RECTANGULAR_ENT_H
#define RECTANGULAR_ENT_H

#include "SimEnt.h"
#include "CircularEnt.h"
#include "Simulation.h"
#include "Point.h"
#include "MathLib.h"

class RectangularEnt : public SimEnt
{
	public:
		// x, y -> left-bottom corner coords, rotating clockwise
		RectangularEnt(uint16_t id, uint32_t weight, bool movable, double x,
			double y, double width, double height, double angle = 0);
		RectangularEnt(std::ifstream& file);

		virtual ~RectangularEnt() { delete _bottLeft; delete _center; }

		double collisionLength(SimEnt& other, Point& proj);
		Point& getBottLeft() { return *_bottLeft; }
		Point& getCenter() { return *_center; }

		virtual void translate(int x, int y);

		virtual void serialize(Buffer& buffer);
		virtual void serialize(std::ofstream& file);

	protected:
		double check_and_divide(CircularEnt& other, Point& bottLeft, double width, double height, int level);

		Point* _bottLeft;
		Point* _center;
		double _width;
		double _height;
		double _angle; // in degrees, rotating clockwise
};

#endif
