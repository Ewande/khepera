#include "CircularEnt.h"

CircularEnt::CircularEnt(uint16_t id, uint32_t weight, bool movable, uint32_t x, uint32_t y,
	uint32_t radius) : SymEnt(id, SymEnt::CIRCLE, weight, movable),
	_x(x), _y(y), _radius(radius)
{
}

void CircularEnt::Translate(int x, int y)
{
	_x += x;
	_y += y;
}