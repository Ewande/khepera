#include "Symulation.h"

Symulation::Symulation(unsigned int areaWidth, unsigned int areaHeight) :
	_areaWidth(areaWidth), _areaHeight(areaHeight)
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