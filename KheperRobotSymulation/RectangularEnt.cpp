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

Buffer* RectangularEnt::Serialize()
{
	int data_length = sizeof(_shapeID) +sizeof(_id) +sizeof(_movable) +sizeof(_weight) +
		sizeof(_x) +sizeof(_y) +sizeof(_width) + sizeof(_height);

	Buffer* result = new Buffer(data_length);

	result->Pack(_shapeID);
	result->Pack(htons(_id));
	result->Pack(_movable);
	result->Pack(htonl(_weight));
	result->Pack(htonl(_x));
	result->Pack(htonl(_y));
	result->Pack(htonl(_width));
	result->Pack(htonl(_height));

	return result;
}