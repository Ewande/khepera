#ifndef CIRCULAR_ENT_H
#define CIRCULAR_ENT_H

#include "SimEnt.h"
#include "Point.h"

class CircularEnt : public SimEnt
{
	public:
		// x, y -> center coords
		CircularEnt(uint16_t id, uint32_t weight, bool movable, double x,
			double y, double radius);
		CircularEnt(std::ifstream& file);
		virtual ~CircularEnt() { delete _center; }


		double collisionLength(SimEnt& other, Point& proj);
		double getX() { return _center->getX(); }
		double getY() { return _center->getY(); }
		double getRadius() { return _radius; }
		Point& getCenter() { return *_center; }

		virtual void translate(int x, int y);

		virtual void serialize(Buffer& buffer);
		virtual void serialize(std::ofstream& file);

	protected:
		Point* _center;
		double _radius;
};

#endif
