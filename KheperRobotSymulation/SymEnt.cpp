#include "SymEnt.h"

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

void SymEnt::Serialize(Buffer& buffer)
{
	buffer.Pack(_shapeID);
	buffer.Pack(htons(_id));
	buffer.Pack(_movable);
	buffer.Pack(htonl(_weight));
}

void SymEnt::Serialize(std::ofstream& file)
{
	file.write(reinterpret_cast<const char*>(&_shapeID), sizeof(_shapeID));
	file.write(reinterpret_cast<const char*>(&_id), sizeof(_id));
	file.write(reinterpret_cast<const char*>(&_movable), sizeof(_movable));
	file.write(reinterpret_cast<const char*>(&_weight), sizeof(_weight));
}