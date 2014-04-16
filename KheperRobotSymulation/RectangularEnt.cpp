#include "RectangularEnt.h"

RectangularEnt::RectangularEnt(uint16_t id, uint32_t weight, bool movable, double x,
	double y, double width, double height) :
	RectangularEnt(id, weight, movable,
	x, y,
	x + width, y,
	x + width, y + height,
	x, y + height)
{
}

RectangularEnt::RectangularEnt(uint16_t id, uint32_t weight, bool movable, double x1,
		double y1, double x2, double y2, double x3, double y3,
		double x4, double y4) : SymEnt(id, SymEnt::RECTANGLE, weight, movable)
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
		Serialization format (integers in network-byte-order, doubles and floats in host-byte-order)

		+-------------------+--------------------------------------+-------------------+
		|                   |                                      |                   |
		|   SHAPE_ID        |              ENTITY_ID               |    MOVABLE        |
		|    8 bytes        |               16 bytes               |    8 bytes        |
		+-------------------+--------------------------------------+-------------------+
		|                                                                              |
		|                                 WEIGHT                                       |
		|                                32 bytes                                      |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                                                              |
		|                                   X1                                         |
		|                                64 bytes                                      |
		|                                                                              |
		|                                                                              |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                                                              |
		|                                   Y1                                         |
		|                                64 bytes                                      |
		|                                                                              |
		|                                                                              |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                                                              |
		|                                   X2                                         |
		|                                64 bytes                                      |
		|                                                                              |
		|                                                                              |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                                                              |
		|                                   Y2                                         |
		|                                64 bytes                                      |
		|                                                                              |
		|                                                                              |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                                                              |
		|                                   X3                                         |
		|                                64 bytes                                      |
		|                                                                              |
		|                                                                              |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                                                              |
		|                                   Y3                                         |
		|                                64 bytes                                      |
		|                                                                              |
		|                                                                              |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                                                              |
		|                                   X4                                         |
		|                                64 bytes                                      |
		|                                                                              |
		|                                                                              |
		+------------------------------------------------------------------------------+
		|                                                                              |
		|                                                                              |
		|                                   Y4                                         |
		|                                64 bytes                                      |
		|                                                                              |
		|                                                                              |
		+------------------------------------------------------------------------------+

							DATA_LENGTH = 576 bytes

*/

void RectangularEnt::Serialize(Buffer& buffer)
{
	SymEnt::Serialize(buffer);

	for (int i = 0; i < 4; i++)
	{
		buffer.Pack(_vertices[i].GetX());
		buffer.Pack(_vertices[i].GetY());
	}
}

void RectangularEnt::Serialize(std::ofstream& file)
{
	SymEnt::Serialize(file);

	for (int i = 0; i < 4; i++)
	{
		double tmp = _vertices[i].GetX();
		file.write(reinterpret_cast<const char*>(&tmp), sizeof(tmp));

		tmp = _vertices[i].GetY();
		file.write(reinterpret_cast<const char*>(&tmp), sizeof(tmp));
	}
}
