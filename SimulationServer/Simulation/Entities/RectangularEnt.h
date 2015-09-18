#ifndef RECTANGULAR_ENT_H
#define RECTANGULAR_ENT_H

#include "SimEnt.h"
#include "CircularEnt.h"
#include "../Constants.h"
#include "../Math/Point.h"

class RectangularEnt : public SimEnt
{
	public:
		// x, y -> left-bottom corner coords, rotating clockwise
		RectangularEnt(uint16_t id, uint32_t weight, bool movable, double x,
			double y, double width, double height, float angle = 0);
        RectangularEnt(std::ifstream& file, bool readBinary);
        RectangularEnt(const RectangularEnt& other);

		virtual ~RectangularEnt() { delete _bottLeft; delete _center; }

		double collisionLength(SimEnt& other, Point& proj);
		Point& getBottLeft() { return *_bottLeft; }
		Point& getCenter() { return *_center; }

		virtual void translate(double x, double y);

		virtual void serialize(Buffer& buffer);
		virtual void serialize(std::ofstream& file);

	protected:
		double check_and_divide(CircularEnt& other, Point& bottLeft, double width, double height, int level);

		Point* _bottLeft;
		Point* _center;
		double _width;
		double _height;
		float _angle; // in radians, rotating clockwise

    private:
        void initializeEntity(double bottLeftX, double bottLeftY);
};

#endif
