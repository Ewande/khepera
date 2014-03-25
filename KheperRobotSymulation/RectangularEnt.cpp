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
	uint32_t x4, uint32_t y4) : SymEnt(id, SymEnt::RECTANGLE, weight, movable)
{
	_vertices = new Point[4];
	_vertices[0] = Point(x1, y1);
	_vertices[1] = Point(x2, y2);
	_vertices[2] = Point(x3, y3);
	_vertices[3] = Point(x4, y4);
}

double RectangularEnt::CollisionLength(SymEnt& other, Point& proj)
{
	int other_shape = other.GetShapeID();
	if (other_shape == SymEnt::CIRCLE || other_shape == SymEnt::KHEPERA_ROBOT)
		return other.CollisionLength(*this, proj);
	else
		return -1;
}

void RectangularEnt::Translate(int x, int y)
{
	for (int i = 0; i < 4; i++)
		_vertices[i].Translate(x, y);
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

	for (int i = 0; i < 4; i++)
	{
		buffer.Pack(htonl(_vertices[i].iGetX()));
		buffer.Pack(htonl(_vertices[i].iGetY()));
	}
}
