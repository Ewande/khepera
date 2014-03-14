#include "RectangularEnt.h"

RectangularEnt::RectangularEnt(uint16_t id, uint32_t weight, bool movable, uint32_t x, uint32_t y,
	uint32_t width, uint32_t height) :
	RectangularEnt(id, weight, movable,
	x, y,
	x + width, y,
	x + width, y + height,
	x, y + height)
{
}

RectangularEnt::RectangularEnt(uint16_t id, uint32_t weight, bool movable, uint32_t x1,
	uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3,
	uint32_t x4, uint32_t y4) : SymEnt(id, SymEnt::RECTANGLE, weight, movable), _x1(x1),
	_y1(y1), _x2(x2), _y2(y2), _x3(x3), _y3(y3), _x4(x4), _y4(y4)
{

}

void RectangularEnt::Translate(int x, int y)
{
	_x1 += x;
	_y1 += y;

	_x2 += x;
	_y2 += y;

	_x3 += x;
	_y3 += y;

	_x4 += x;
	_y4 += y;
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
		|                                   X1                                         |
		|                                32 bytes                                      |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                   Y1                                         |
		|                                32 bytes                                      |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                   X2                                         |
		|                                32 bytes                                      |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                   Y2                                         |
		|                                32 bytes                                      |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                   X3                                         |
		|                                32 bytes                                      |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                   Y3                                         |
		|                                32 bytes                                      |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                   X4                                         |
		|                                32 bytes                                      |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                   Y4                                         |
		|                                32 bytes                                      |
		+------------------------------------------------------------------------------+

							DATA_LENGTH = 320 bytes

*/

void RectangularEnt::Serialize(Buffer& buffer)
{
	buffer.Pack(_shapeID);
	buffer.Pack(htons(_id));
	buffer.Pack(_movable);
	buffer.Pack(htonl(_weight));

	buffer.Pack(htonl(_x1));
	buffer.Pack(htonl(_y1));

	buffer.Pack(htonl(_x2));
	buffer.Pack(htonl(_y2));

	buffer.Pack(htonl(_x3));
	buffer.Pack(htonl(_y3));

	buffer.Pack(htonl(_x4));
	buffer.Pack(htonl(_y4));
}
