#ifndef RECTANGULAR_ENT
#define RECTANGULAR_ENT

#include "SimEnt.h"

class RectangularEnt : public SimEnt
{
	public:
		RectangularEnt(uint16_t id, uint32_t weight, bool movable, uint32_t x,
			uint32_t y, uint32_t width, uint32_t height);

		virtual void Translate(int x, int y);
	private:
		uint32_t _x;
		uint32_t _y;
		uint32_t _width;
		uint32_t _height;
};

#endif