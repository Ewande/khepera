#include "CircularEnt.h"
#include "RectangularEnt.h"
#include "LinearEnt.h"

#include <iostream>

CircularEnt::CircularEnt(uint16_t id, uint32_t weight, bool movable, double x, double y,
	double radius) : SymEnt(id, SymEnt::CIRCLE, weight, movable), _radius(radius)
{
	_center = new Point(x, y);
}


double CircularEnt::CollisionLength(SymEnt& other, Point& proj)
{
	int other_shape = other.GetShapeID();

	if (other_shape == LINE)
	{

		/*

		- (x1, y1) and (x2, y2) are two ends of the line segment
		- (_x, _y) is the center of current CircularEnt
		- (x, y) is an orthogonal projection of point (_x, _y) into the line segment

		If distance between (_x, _y) and (x, y) is shorter than the radius and if (x, y) belongs to
		the line segment (0 <= u <= 1), there is a collision.

		There is also a possibility that (x, y) doesn't belong to the line segment and there is a collision.
		To check it we just have to calculate distance between both ends of the line segment and the center of
		CircularEnt. If any of these distances is shorter than the radius, there is a collision.

		*/

		double _x = GetX();
		double _y = GetY();
		LinearEnt &converted = *dynamic_cast<LinearEnt*>(&other);

		double x1 = converted.GetBeg().GetX(),
			y1 = converted.GetBeg().GetY();
		double x2 = converted.GetEnd().GetX(),
			y2 = converted.GetEnd().GetY();

		double u = ((_x - x1) * (x2 - x1) + (_y - y1) * (y2 - y1)) /
			((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

		double x = x1 + (x2 - x1) * u;
		double y = y1 + (y2 - y1) * u;
		double x_diff = x - _x;
		double y_diff = y - _y;
		double ovr_diff = sqrt(x_diff * x_diff + y_diff * y_diff);

		if (ovr_diff <= _radius)
		{
			if (u >= 0 && u <= 1)
			{
				proj.SetCords(x, y);
				return _radius - ovr_diff + 1;
			}

			double dist_to_vertex = converted.GetBeg().GetDistance(*_center);
			if (dist_to_vertex <= _radius)
			{
				proj.SetCords(converted.GetBeg());
				return _radius - dist_to_vertex + 1;
			}

			dist_to_vertex = converted.GetEnd().GetDistance(*_center);
			if (dist_to_vertex <= _radius)
			{
				proj.SetCords(converted.GetEnd());
				return _radius - dist_to_vertex + 1;
			}
		}

		return -1;
	}

	else if (other_shape == RECTANGLE)
	{
		return other.CollisionLength(*this, proj);
	}

	else if (other_shape == CIRCLE || other_shape == KHEPERA_ROBOT)
	{
		CircularEnt &converted = *dynamic_cast<CircularEnt*>(&other);
		double radiuses_sum = _radius + converted.GetRadius();
		double centres_diff = _center->GetDistance(converted.GetCenter());

		return radiuses_sum - centres_diff + 1;
	}

	return -1;
}

void CircularEnt::Translate(int x, int y)
{
	_center->Translate(x, y);
}

/*
		 Serialization format (integers in network-byte-order, doubles and floats in host-byte-order)

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
			 |                                                                              |
			 |                                X COORD                                       |
			 |                                64 bytes                                      |
			 |                                                                              |
			 |                                                                              |
			 +------------------------------------------------------------------------------+
			 |                                                                              |
			 |                                                                              |
			 |                                Y COORD                                       |
			 |                                64 bytes                                      |
			 |                                                                              |
			 |                                                                              |
			 +------------------------------------------------------------------------------+
			 |                                                                              |
			 |                                                                              |
			 |                                 RADIUS                                       | 
			 |                                64 bytes                                      |
			 |                                                                              |
			 |                                                                              |
			 +------------------------------------------------------------------------------+

			 DATA_LENGTH = 256 bytes

*/

void CircularEnt::Serialize(Buffer& buffer)
{
	buffer.Pack(_shapeID);
	buffer.Pack(htons(_id));
	buffer.Pack(_movable);
	buffer.Pack(htonl(_weight));
	buffer.Pack(GetX());
	buffer.Pack(GetY());
	buffer.Pack(_radius);
}
