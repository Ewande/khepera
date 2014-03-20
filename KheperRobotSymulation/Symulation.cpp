#include "Symulation.h"

DWORD WINAPI SymulationThreadWrapperFunction(LPVOID threadData)
{
	Symulation* sym = static_cast<Symulation*>(threadData);

	sym->Run();

	return 0; // unused thread return value
}

Symulation::Symulation(unsigned int worldWidth, unsigned int worldHeight) :
	_worldWidth(worldWidth), _worldHeight(worldHeight), _isRunning(false),
	_commMan(NULL), _symulationThreadHandle(INVALID_HANDLE_VALUE)
{
}

Symulation::~Symulation()
{
	_isRunning = false; // to stop _symulationThreadHandle

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
	_isRunning = true;

	_symulationThreadHandle = CreateThread(NULL, 0, SymulationThreadWrapperFunction,
		static_cast<LPVOID>(this), 0, NULL);
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

void Symulation::Run()
{
	int i = 0;
	while (_isRunning)
	{
		_commMan->SendWorldDescriptionToVisualisers();
		Sleep(125);
		Update(1);
		std::cout << "RUNNING" <<  i++ << std::endl;
	}

	std::cout << "THREAD END" << std::endl;
}