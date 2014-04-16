#ifndef CIRCULAR_ENT_H
#define CIRCULAR_ENT_H

#include "SymEnt.h"
#include "Point.h"

class CircularEnt : public SymEnt
{
	public:
		CircularEnt(uint16_t id, uint32_t weight, bool movable, double x,
			double y, double radius);
		virtual ~CircularEnt() { delete _center; }


		double CollisionLength(SymEnt& other, Point& proj);
		double GetX() { return _center->GetX(); }
		double GetY() { return _center->GetY(); }
		double GetRadius() { return _radius; }
		Point& GetCenter() { return *_center; }

		virtual void Translate(int x, int y);

		virtual void Serialize(Buffer& buffer);
		virtual void Serialize(std::ofstream& file);

	protected:
		Point* _center;
		double _radius;
};

#endif
