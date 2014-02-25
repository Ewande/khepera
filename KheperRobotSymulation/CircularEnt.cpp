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
			 |                                RADIUS                                        |
			 |                                32 bytes                                      |
			 +------------------------------------------------------------------------------+

			 DATA_LENGTH = 160 bytes

*/

Buffer* CircularEnt::Serialize()
{
	int data_length = sizeof(_shapeID) +sizeof(_id) +sizeof(_movable) +sizeof(_weight) +
		sizeof(_x) +sizeof(_y) +sizeof(_radius);

	Buffer* result = new Buffer(data_length);

	result->Pack(_shapeID);
	result->Pack(htons(_id));
	result->Pack(_movable);
	result->Pack(htonl(_weight));
	result->Pack(htonl(_x));
	result->Pack(htonl(_y));
	result->Pack(htonl(_radius));

	return result;
}
