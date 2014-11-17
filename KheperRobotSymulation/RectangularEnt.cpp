#include "RectangularEnt.h"

RectangularEnt::RectangularEnt(uint16_t id, uint32_t weight, bool movable, double x,
	double y, double width, double height, double angle) : SymEnt(id, SymEnt::RECTANGLE, weight, movable),
	_width(width), _height(height), _angle(angle)
{
	_bottLeft = new Point(x, y);

	double ang_cos = cosD(_angle);
	double ang_sin = sinD(_angle);
	_center = new Point(_bottLeft->GetX() + _width / 2.0 * ang_cos + _height / 2.0 * ang_sin,
		_bottLeft->GetY() - _width / 2.0 * ang_sin + _height / 2.0 * ang_cos);
}

/*RectangularEnt::RectangularEnt(uint16_t id, uint32_t weight, bool movable, double x1,
		double y1, double x2, double y2, double x3, double y3,
		double x4, double y4) : SymEnt(id, SymEnt::RECTANGLE, weight, movable)
{
	_vertices = new Point[4];
	_vertices[0] = Point(x1, y1);
	_vertices[1] = Point(x2, y2);
	_vertices[2] = Point(x3, y3);
	_vertices[3] = Point(x4, y4);
}*/

double RectangularEnt::CollisionLength(SymEnt& other, Point& proj)
{
	int other_shape = other.GetShapeID();
	if (other_shape == SymEnt::CIRCLE || other_shape == SymEnt::KHEPERA_ROBOT)
	{
		//std::cout << "Startujemy: pozycja x=" << _bottLeft->GetX() << ", y=" << _bottLeft->GetY() << "\n";
		CircularEnt &converted = *dynamic_cast<CircularEnt*>(&other);
		Point *clone = new Point(*_bottLeft);
		double coll_len = check_and_divide(converted, *clone, _width, _height, 1);

		delete clone;
		return coll_len;
	}
	else
		return -1;
}

void RectangularEnt::Translate(int x, int y)
{
	_bottLeft->Translate(x, y);
}

double RectangularEnt::check_and_divide(CircularEnt& other, Point& bottLeft, double width, double height, int level)
{
	if (level > DIVIDING_LEVEL)
		return INF_COLLISION;

	double ang_cos = cosD(_angle);
	double ang_sin = sinD(_angle);
	width /= 2.0;
	height /= 2.0;

	Point center(bottLeft.GetX() + width * ang_cos + height * ang_sin,
		bottLeft.GetY() + width * ang_sin + height * ang_cos);
	double radius = center.GetDistance(bottLeft);

	double radiuses_sum = radius + other.GetRadius();
	double centres_diff = center.GetDistance(other.GetCenter());

	if (centres_diff > radiuses_sum)
	{
		//std::cout << "Brak kolizji na poz. " << level << ", promien: " << radius << "\n";
		return NO_COLLISION;
	}
	else
	{
		double max_coll = NO_COLLISION;
		level++;

		Point copy = Point(bottLeft);

		max_coll = max(max_coll, check_and_divide(other, bottLeft, width, height, level));
		//std::cout << "lewy dolny: " << bottLeft.GetX() << " " << bottLeft.GetY() << "\n";
		bottLeft.Translate(width * ang_cos, -width * ang_sin);
		max_coll = max(max_coll, check_and_divide(other, bottLeft, width, height, level));
		//std::cout << "prawy dolny: " << bottLeft.GetX() << " " << bottLeft.GetY() << "\n";
		bottLeft.SetCords(copy);
		bottLeft.Translate(height * ang_sin, height * ang_cos);
		max_coll = max(max_coll, check_and_divide(other, bottLeft, width, height, level));
		//std::cout << "lewy gorny: " << bottLeft.GetX() << " " << bottLeft.GetY() << "\n";
		max_coll = max(max_coll, check_and_divide(other, center, width, height, level));
		//std::cout << "prawy gorny: " << bottLeft.GetX() << " " << bottLeft.GetY() << "\n";
		bottLeft.SetCords(copy);
		//if (max_coll < INF_COLLISION)
			//std::cout << level << ": " << max_coll << ", suma promieni - odleglosc srodkow" <<
			//radiuses_sum - centres_diff << std::endl;

		return min(max_coll, radiuses_sum - centres_diff) + 1;
	}
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
	buffer.Pack(_shapeID);
	buffer.Pack(htons(_id));
	buffer.Pack(_movable);
	buffer.Pack(htonl(_weight));
	buffer.Pack(_bottLeft->GetX());
	buffer.Pack(_bottLeft->GetY());
	buffer.Pack(_bottLeft->GetX() + _width * cos(M_PI * _angle / 180.0));
	buffer.Pack(_bottLeft->GetY() - _width * sin(M_PI * _angle / 180.0));
	buffer.Pack(_bottLeft->GetX() + _width * cos(M_PI * _angle / 180.0) + _height * sin(M_PI * _angle / 180.0));
	buffer.Pack(_bottLeft->GetY() - _width * sin(M_PI * _angle / 180.0) + _height * cos(M_PI * _angle / 180.0));
	buffer.Pack(_bottLeft->GetX() + _height * sin(M_PI * _angle / 180.0));
	buffer.Pack(_bottLeft->GetY() + _height * cos(M_PI * _angle / 180.0));
}
