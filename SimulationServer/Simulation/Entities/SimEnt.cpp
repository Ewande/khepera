#include "SimEnt.h"

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
	|                            SHAPE_SPECIFIC_DATA                               |
	|                              variable length                                 |
	+------------------------------------------------------------------------------+

*/

SimEnt::SimEnt(std::ifstream& file, bool readBinary, uint8_t shapeID)
{
	_shapeID = shapeID;
    if (readBinary)
    {
        file.read(reinterpret_cast<char*>(&_id), sizeof(_id));
        file.read(reinterpret_cast<char*>(&_movable), sizeof(_movable));
        file.read(reinterpret_cast<char*>(&_weight), sizeof(_weight));
    }
    else
    {
        uint16_t movable16;
        file >> _id >> movable16 >> _weight;
        _movable = (uint8_t) movable16;
    }
}

void SimEnt::serialize(Buffer& buffer)
{
	buffer.pack(_shapeID);
	buffer.pack(htons(_id));
	buffer.pack(_movable);
	buffer.pack(htonl(_weight));
}

void SimEnt::serialize(std::ofstream& file)
{
	file.write(reinterpret_cast<const char*>(&_shapeID), sizeof(_shapeID));
	file.write(reinterpret_cast<const char*>(&_id), sizeof(_id));
	file.write(reinterpret_cast<const char*>(&_movable), sizeof(_movable));
	file.write(reinterpret_cast<const char*>(&_weight), sizeof(_weight));
}