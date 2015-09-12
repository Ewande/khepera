#ifndef LINEAR_ENT_H
#define LINEAR_ENT_H

#include "../Constants.h"
#include "SimEnt.h"
#include "../Math/Point.h"

class LinearEnt : public SimEnt
{
    public:
	    LinearEnt(uint16_t id, double begX, double begY, double endX, double endY);
        LinearEnt(std::ifstream& file, bool readBinary);
	    ~LinearEnt();

	    Point& getBeg() { return *_beg; }
	    Point& getEnd() { return *_end; }
	    double getLength() { return _length; }

	    double collisionLength(SimEnt& other, Point& proj);
	    void translate(double x, double y);

	    void serialize(Buffer& buffer);
        void serialize(std::ofstream& file);

    private:
        void initializeEntity(double begX, double begY, double endX, double endY);

	    Point* _beg;
	    Point* _end;
	    double _length;
    
};

#endif
