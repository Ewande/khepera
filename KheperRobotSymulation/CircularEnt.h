#ifndef CIRCULAR_ENT_H
#define CIRCULAR_ENT_H

#include "SimEnt.h"

class CircularEnt : public SimEnt
{
	public:
		CircularEnt(uint16_t id, uint32_t weight, bool movable, uint32_t x,
			uint32_t y, uint32_t radius);

		virtual void Translate(int x, int y);

	private:
		uint32_t _x;
		uint32_t _y;
		uint32_t _radius;
};

#endif
