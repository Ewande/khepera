#include "LinearEnt.h"

LinearEnt::LinearEnt(uint16_t id, double begX, double begY,
	double endX, double endY) : SimEnt(id, SimEnt::LINE, 0, false)
{
    initializeEntity(begX, begY, endX, endY);
}

LinearEnt::LinearEnt(std::ifstream& file, bool readBinary) : SimEnt(file, readBinary, SimEnt::LINE)
{
    double begX, begY, endX, endY;
    if (readBinary)
    {
        file.read(reinterpret_cast<char*>(&begX), sizeof(begX));
        file.read(reinterpret_cast<char*>(&begY), sizeof(begY));
        file.read(reinterpret_cast<char*>(&endX), sizeof(endX));
        file.read(reinterpret_cast<char*>(&endY), sizeof(endY));
    }
    else
        file >> begX >> begY >> endX >> endY;

    initializeEntity(begX, begY, endX, endY);
}

LinearEnt::LinearEnt(const LinearEnt& other) : SimEnt(other)
{
    _beg = new Point(*other._beg);
    _end = new Point(*other._end);
    _length = other._length;
}

void LinearEnt::initializeEntity(double begX, double begY, double endX, double endY)
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
    SimEnt::serialize(buffer);
	buffer.pack(_beg->getX());
	buffer.pack(_beg->getY());
	buffer.pack(_end->getX());
	buffer.pack(_end->getY());
}

void LinearEnt::serialize(std::ofstream& file)
{
    SimEnt::serialize(file);

    double begX = _beg->getX();
    double begY = _beg->getY();
    double endX = _end->getX();
    double endY = _end->getY();
    file.write(reinterpret_cast<const char*>(&begX), sizeof(begX));
    file.write(reinterpret_cast<const char*>(&begY), sizeof(begY));
    file.write(reinterpret_cast<const char*>(&endX), sizeof(endX));
    file.write(reinterpret_cast<const char*>(&endY), sizeof(endY));
}