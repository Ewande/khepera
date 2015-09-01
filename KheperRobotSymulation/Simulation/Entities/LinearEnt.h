#ifndef LINEAR_ENT_H
#define LINEAR_ENT_H

#include "../Simulation.h"
#include "SimEnt.h"
#include "../Math/Point.h"

class LinearEnt : public SimEnt
{
public:
	LinearEnt(uint16_t id, double begX, double begY, double endX, double endY);
	~LinearEnt();

	Point& getBeg() { return *_beg; }
	Point& getEnd() { return *_end; }
	double getLength() { return _length; }

	double collisionLength(SimEnt& other, Point& proj);

	virtual void translate(double x, double y);

	virtual void serialize(Buffer& buffer);
	virtual void serialize(std::ofstream& file) {}

protected:
	Point* _beg;
	Point* _end;
	double _length;
};

#endif
