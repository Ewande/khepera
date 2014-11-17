#include "LinearEnt.h"

LinearEnt::LinearEnt(uint16_t id, double begX, double begY,
	double endX, double endY) : SymEnt(id, SymEnt::LINE, 0, false)
{
	_beg = new Point(begX, begY);
	_end = new Point(endX, endY);
	_length = _beg->GetDistance(*_end);
}

LinearEnt::~LinearEnt()
{
	delete _beg;
	delete _end;
}


double LinearEnt::CollisionLength(SymEnt& other, Point& proj)
{
	int other_shape = other.GetShapeID();

	if (other_shape == SymEnt::CIRCLE || other_shape == SymEnt::KHEPERA_ROBOT)
		return other.CollisionLength(*this, proj);
	else
		return -1;
}

void LinearEnt::Translate(int x, int y)
{
	_beg->Translate(x, y);
	_end->Translate(x, y);
}

void LinearEnt::Serialize(Buffer& buffer)
{
	buffer.Pack(_shapeID);
	buffer.Pack(htons(_id));
	buffer.Pack(_movable);
	buffer.Pack(htonl(_weight));
	buffer.Pack(_beg->GetX());
	buffer.Pack(_beg->GetY());
	buffer.Pack(_end->GetX());
	buffer.Pack(_end->GetY());
}
