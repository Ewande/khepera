#include "RectangularEnt.h"

RectangularEnt::RectangularEnt(uint16_t id, uint32_t weight, bool movable, uint32_t x, uint32_t y,
	uint32_t width, uint32_t height) : SymEnt(id, SymEnt::RECTANGLE, weight, movable),
	_x(x), _y(y), _width(width), _height(height)
{
}

void RectangularEnt::Translate(int x, int y)
{
	_x += x;
	_y += y;
}

/*
			 		Serialization format (all numbers in network byte order)
		+-------------------+--------------------------------------+-------------------+
		|                   |                                      |                   |
		|   SHAPE_ID        |              ENTITY_ID               |    MOVABLE        |
		|    8 bytes        |               16 bytes               |    8 bytes        |
		+-------------------+--------------------------------------+-------------------+
		|                                                                              |
		|                                WEIGHT                                        |
		|                                32 bytes                                      |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                X COORD                                       |
		|                                32 bytes                                      |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                y COORD                                       |
		|                                32 bytes                                      |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                 WIDTH                                        |
		|                                32 bytes                                      |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                 HEIGHT                                       |
		|                                32 bytes                                      |
		+------------------------------------------------------------------------------+

							DATA_LENGTH = 192 bytes

*/

void RectangularEnt::Serialize(Buffer& buffer)
{
	buffer.Pack(_shapeID);
	buffer.Pack(htons(_id));
	buffer.Pack(_movable);
	buffer.Pack(htonl(_weight));
	buffer.Pack(htonl(_x));
	buffer.Pack(htonl(_y));
	buffer.Pack(htonl(_width));
	buffer.Pack(htonl(_height));
}