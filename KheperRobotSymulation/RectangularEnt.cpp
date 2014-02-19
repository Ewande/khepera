#include "RectangularEnt.h"

RectangularEnt::RectangularEnt(uint16_t id, uint32_t weight, bool movable, uint32_t x, uint32_t y,
	uint32_t width, uint32_t height) : SimEnt(id, SimEnt::RECTANGLE, weight, movable),
	_x(x), _y(y), _width(width), _height(height)
{
}

void RectangularEnt::Translate(int x, int y)
{
	_x += x;
	_y += y;
}