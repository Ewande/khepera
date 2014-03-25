#ifndef RECTANGULAR_ENT_H
#define RECTANGULAR_ENT_H

#include "SymEnt.h"
#include "Point.h"

class RectangularEnt : public SymEnt
{
	public:
		// x, y -> coord of left-top corner
		RectangularEnt(uint16_t id, uint32_t weight, bool movable, uint32_t x,
			uint32_t y, uint32_t width, uint32_t height);

		// points should be in clockwise order
		RectangularEnt(uint16_t id, uint32_t weight, bool movable, uint32_t x1,
			uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3,
			uint32_t x4, uint32_t y4);

		~RectangularEnt() { delete _vertices; }

		double CollisionLength(SymEnt& other, Point& proj);
		Point* GetVertices() { return _vertices; }


		virtual void Translate(int x, int y);

		virtual void Serialize(Buffer& buffer);

	protected:
		Point* _vertices;
};

#endif
