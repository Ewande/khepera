#include "Symulation.h"

Symulation::Symulation(unsigned int worldWidth, unsigned int worldHeight) :
	_worldWidth(worldWidth), _worldHeight(worldHeight)
{
}

Symulation::~Symulation()
{
	std::map<uint16_t, SymEnt*>::iterator it = _entities.begin();

	while (it != _entities.end())
	{
		delete it->second;
		it++;
	}
}

void Symulation::AddEntity(SymEnt* newEntity)
{
	_entities[newEntity->GetID()] = newEntity;
}

void Symulation::Start()
{
	_time = 0;
}

void Symulation::Update(unsigned int deltaTime)
{
	std::map<uint16_t, SymEnt*>::iterator it = _entities.begin();

	_time += deltaTime;

	while (it != _entities.end())
	{
		if (it->second->GetShapeID() == SymEnt::KHEPERA_ROBOT)
		{
			dynamic_cast<KheperaRobot*>(it->second)->UpdatePosition(deltaTime);
		}
		it++;
	}
}


/*
					Serialization format (all numbers in network byte order)
			+-------------------+--------------------------------------+-------------------+
			|                                                                              |
			|                              WORLD_WIDTH                                     |
			|                                32 bytes                                      |
			+------------------------------------------------------------------------------+
			|                                                                              |
			|                              WORLD_HEIGHT                                    |
			|                                32 bytes                                      |
			+------------------------------------------------------------------------------+
			|                                                                              |
			|                                  TIME                                        |
			|                                32 bytes                                      |
			+-------------------+--------------------------------------+-------------------+
			|                                      |                                       |
			|          NUMBER_OF_ENTITIES          |              ENTITIES_DATA            |
			|               16 bytes               |              variable length          |
			+--------------------------------------+---------------------------------------+

*/
void Symulation::Serialize(Buffer& buffer) const
{
	buffer.Pack(htonl(_worldWidth));
	buffer.Pack(htonl(_worldHeight));
	buffer.Pack(htonl(_time));
	buffer.Pack(htons(static_cast<uint16_t>(_entities.size())));

	std::map<uint16_t, SymEnt*>::const_iterator it = _entities.begin();

	while (it != _entities.end())
	{
		it->second->Serialize(buffer);
		it++;
	}
}