#include "LinearEnt.h"

LinearEnt::LinearEnt(uint16_t id, double begX, double begY,
	double endX, double endY) : SimEnt(id, SimEnt::LINE, 0, false)
{
	_beg = new Point(begX, begY);
	_end = new Point(endX, endY);
	_length = _beg->getDistance(*_end);
}

LinearEnt::~LinearEnt()
{
	delete _beg;
	delete _end;
}


double LinearEnt::collisionLength(SimEnt& other, Point& proj)
{
	int other_shape = other.getShapeID();

	if (other_shape == SimEnt::CIRCLE || other_shape == SimEnt::KHEPERA_ROBOT)
		return other.collisionLength(*this, proj);
	else
		return NO_COLLISION;
}

void LinearEnt::translate(double x, double y)
{
	_beg->translate(x, y);
	_end->translate(x, y);
}

void LinearEnt::serialize(Buffer& buffer)
{
	buffer.pack(_shapeID);
	buffer.pack(htons(_id));
	buffer.pack(_movable);
	buffer.pack(htonl(_weight));
	buffer.pack(_beg->getX());
	buffer.pack(_beg->getY());
	buffer.pack(_end->getX());
	buffer.pack(_end->getY());
}
