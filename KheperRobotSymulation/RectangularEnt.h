#ifndef RECTANGULAR_ENT_H
#define RECTANGULAR_ENT_H

#include "SymEnt.h"
#include "CircularEnt.h"
#include "Symulation.h"
#include "Point.h"
#include "MathLib.h"

class RectangularEnt : public SymEnt
{
	public:
		// x, y -> left-bottom corner coords, rotating clockwise
		RectangularEnt(uint16_t id, uint32_t weight, bool movable, double x,
			double y, double width, double height, double angle = 0);
		RectangularEnt(std::ifstream& file);

		virtual ~RectangularEnt() { delete _bottLeft; delete _center; }

		double CollisionLength(SymEnt& other, Point& proj);
		Point& GetBottLeft() { return *_bottLeft; }
		Point& GetCenter() { return *_center; }

		virtual void Translate(int x, int y);

		virtual void Serialize(Buffer& buffer);
		virtual void Serialize(std::ofstream& file);

	protected:
		double check_and_divide(CircularEnt& other, Point& bottLeft, double width, double height, int level);

		Point* _bottLeft;
		Point* _center;
		double _width;
		double _height;
		double _angle; // in degrees, rotating clockwise
};

#endif
