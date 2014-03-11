#ifndef CIRCULAR_ENT_H
#define CIRCULAR_ENT_H

#include "SymEnt.h"

class CircularEnt : public SymEnt
{
	public:
		CircularEnt(uint16_t id, uint32_t weight, bool movable, uint32_t x,
			uint32_t y, uint32_t radius);

		virtual void Translate(int x, int y);

		virtual void Serialize(Buffer& buffer);

	protected:
		uint32_t _x;
		uint32_t _y;
		uint32_t _radius;
};

#endif
