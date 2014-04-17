#ifndef RECTANGULAR_ENT_H
#define RECTANGULAR_ENT_H

#include "SymEnt.h"
#include "Point.h"

class RectangularEnt : public SymEnt
{
	public:
		// x, y -> coord of left-top corner
		RectangularEnt(uint16_t id, uint32_t weight, bool movable, double x,
			double y, double width, double height);
		RectangularEnt(std::ifstream& file);

		// points should be in clockwise order
		RectangularEnt(uint16_t id, uint32_t weight, bool movable, double x1,
			double y1, double x2, double y2, double x3, double y3,
			double x4, double y4);

		virtual ~RectangularEnt() { delete[] _vertices; }

		double CollisionLength(SymEnt& other, Point& proj);
		Point* GetVertices() { return _vertices; }


		virtual void Translate(int x, int y);

		virtual void Serialize(Buffer& buffer);
		virtual void Serialize(std::ofstream& file);

	protected:
		Point* _vertices;
};

#endif
