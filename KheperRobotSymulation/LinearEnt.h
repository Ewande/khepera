#ifndef LINEAR_ENT_H
#define LINEAR_ENT_H

#include "SymEnt.h"
#include "Point.h"

class LinearEnt : public SymEnt
{
public:
	LinearEnt(uint16_t id, double begX, double begY, double endX, double endY);
	~LinearEnt();

	Point& GetBeg() { return *_beg; }
	Point& GetEnd() { return *_end; }
	double GetLength() { return _length; }

	double CollisionLength(SymEnt& other, Point& proj);

	virtual void Translate(int x, int y);

	virtual void Serialize(Buffer& buffer);
	virtual void Serialize(std::ofstream& file) {}

protected:
	Point* _beg;
	Point* _end;
	double _length;
};

#endif
