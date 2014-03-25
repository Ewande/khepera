#ifndef CIRCULAR_ENT_H
#define CIRCULAR_ENT_H

#include "SymEnt.h"
#include "Point.h"

class CircularEnt : public SymEnt
{
	public:
		CircularEnt(uint16_t id, uint32_t weight, bool movable, uint32_t x,
			uint32_t y, uint32_t radius);
		~CircularEnt() { delete _center; }


		double CollisionLength(SymEnt& other, Point& proj);
		uint32_t GetX() { return _center->iGetX(); }
		uint32_t GetY() { return _center->iGetY(); }
		uint32_t GetRadius() { return _radius; }
		Point& GetCenter() { return *_center; }

		virtual void Translate(int x, int y);

		virtual void Serialize(Buffer& buffer);

	protected:
		Point* _center;
		uint32_t _radius;
};

#endif
