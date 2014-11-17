#include "Symulation.h"
#include <iostream>

DWORD WINAPI SymulationThreadWrapperFunction(LPVOID threadData)
{
	Symulation* sym = static_cast<Symulation*>(threadData);

	sym->Run();

	return 0; // unused thread return value
}

Symulation::Symulation(unsigned int worldWidth, unsigned int worldHeight, bool addBounds) :
	_worldWidth(worldWidth), _worldHeight(worldHeight), _isRunning(false),
	_commMan(NULL), _symulationThreadHandle(INVALID_HANDLE_VALUE), _symulationStep(DEFAULT_SYMULATION_STEP),
	_symulationDelay(DEFAULT_SYMULATION_DELAY)
{
	if (addBounds)
	{
		LinearEnt* bottom_line = new LinearEnt(RESERVED_ID_LEVEL + 1, 0, 0, worldWidth, 0);
		LinearEnt* top_line = new LinearEnt(RESERVED_ID_LEVEL + 2, 0, worldHeight, worldWidth, worldHeight);
		LinearEnt* left_line = new LinearEnt(RESERVED_ID_LEVEL + 3, 0, 0, 0, worldHeight);
		LinearEnt* right_line = new LinearEnt(RESERVED_ID_LEVEL + 4, worldWidth, 0, worldWidth, worldHeight);
		_entities[RESERVED_ID_LEVEL + 1] = bottom_line;
		_entities[RESERVED_ID_LEVEL + 2] = top_line;
		_entities[RESERVED_ID_LEVEL + 3] = left_line;
		_entities[RESERVED_ID_LEVEL + 4] = right_line;
	}
	InitializeCriticalSection(&_criticalSection);
}

Symulation::Symulation(unsigned int worldWidth, unsigned int worldHeight,
	double symulationStep , int symulationDelay) :
	_worldWidth(worldWidth), _worldHeight(worldHeight), _isRunning(false),
	_commMan(NULL), _symulationThreadHandle(INVALID_HANDLE_VALUE), _symulationStep(symulationStep),
	_symulationDelay(symulationDelay)
{
	InitializeCriticalSection(&_criticalSection);
}

Symulation::Symulation(std::ifstream& file, double symulationStep,
	int symulationDelay) : _isRunning(false),
	_commMan(NULL), _symulationThreadHandle(INVALID_HANDLE_VALUE), _symulationStep(symulationStep),
	_symulationDelay(symulationDelay)
{
	uint16_t numberOfEntities;

	file.read(reinterpret_cast<char*>(&_worldWidth), sizeof(_worldWidth));
	file.read(reinterpret_cast<char*>(&_worldHeight), sizeof(_worldHeight));
	file.read(reinterpret_cast<char*>(&_time), sizeof(_time));
	file.read(reinterpret_cast<char*>(&numberOfEntities), sizeof(numberOfEntities));

	for (uint16_t i = 0; i < numberOfEntities; i++)
	{
		uint8_t shapeID;
		file.read(reinterpret_cast<char*>(&shapeID), sizeof(shapeID));
		SymEnt* newEntity;

		switch (shapeID)
		{
			case SymEnt::CIRCLE: newEntity = new CircularEnt(file); break;
			case SymEnt::RECTANGLE: newEntity = new RectangularEnt(file); break;
			case SymEnt::KHEPERA_ROBOT: newEntity = new KheperaRobot(file); break;

			default: newEntity = NULL; /* TODO: Exception handling */
		}
		AddEntity(newEntity);
	}

	InitializeCriticalSection(&_criticalSection);
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

	DeleteCriticalSection(&_criticalSection);
}

void Symulation::AddEntity(SymEnt* newEntity)
{
	int new_id = newEntity->GetID();
	if (new_id < RESERVED_ID_LEVEL)
		_entities[new_id] = newEntity;
}

void Symulation::Start()
{
	_time = 0;
	_isRunning = true;

	_symulationThreadHandle = CreateThread(NULL, 0, SymulationThreadWrapperFunction,
		static_cast<LPVOID>(this), 0, NULL);
}

void Symulation::Update(double deltaTime)
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
	CheckCollisions();
}

void Symulation::CheckCollisions()
{
	std::map<uint16_t, SymEnt*>::iterator it1, it2;
	for (int i = 0; i < NUMBER_OF_CHECKS; i++)
		for (it1 = _entities.begin(); it1 != _entities.end(); it1++)
		{
			int shapeId = it1->second->GetShapeID();

			for (it2 = it1; it2 != _entities.end(); it2++)
			{
				if (it1->second->GetID() != it2->second->GetID())
				{
					// orthogonal projection onto line (used only when sth is colliding with line)
					Point proj;

					double collision_len = it1->second->CollisionLength(*(it2->second), proj);
					if (collision_len >= 0)
					{
						std::cout << "kolizja " << it1->second->GetID() << " i " << it2->second->GetID() << "\n";
						std::cout << "--- dlugosc: " << collision_len << "\n\n";
						removeCollision(*it1->second, *it2->second, collision_len, proj);
					}
				}
			}
		}


}


void Symulation::removeCollision(SymEnt& fst, SymEnt& snd, double collisionLen, Point& proj)
{
	int fst_shape = fst.GetShapeID();
	int snd_shape = snd.GetShapeID();
	if (fst_shape != SymEnt::LINE && snd_shape != SymEnt::LINE)
	{
		Point *center_fst;
		Point *center_snd;
		
		if (fst_shape == SymEnt::RECTANGLE)
		{
			RectangularEnt& conv_fst = *dynamic_cast<RectangularEnt*>(&fst);
			center_fst = &conv_fst.GetCenter();
		}
		else
		{
			CircularEnt& conv_fst = *dynamic_cast<CircularEnt*>(&fst);
			center_fst = &conv_fst.GetCenter();
		}

		if (snd_shape == SymEnt::RECTANGLE)
		{
			RectangularEnt& conv_snd = *dynamic_cast<RectangularEnt*>(&snd);
			center_snd = &conv_snd.GetCenter();
		}
		else
		{
			CircularEnt& conv_snd = *dynamic_cast<CircularEnt*>(&snd);
			center_snd = &conv_snd.GetCenter();
		}

		double weights_sum = fst.GetWeight() + snd.GetWeight();
		double fst_coeff = snd.GetWeight() / weights_sum;
		double snd_coeff = fst.GetWeight() / weights_sum;

		double centers_diff = center_fst->GetDistance(*center_snd);
		double x_diff = center_fst->GetXDiff(*center_snd);
		double y_diff = center_fst->GetYDiff(*center_snd);

		double fst_x_trans = x_diff / centers_diff * collisionLen * fst_coeff;
		double fst_y_trans = y_diff / centers_diff * collisionLen * fst_coeff;

		double snd_x_trans = (-1) * x_diff / centers_diff * collisionLen * snd_coeff;
		double snd_y_trans = (-1) * y_diff / centers_diff * collisionLen * snd_coeff;

		/*if (fst_coeff > snd_coeff)
		{
			fst_x_trans += x_diff_sgn;
			fst_y_trans += y_diff_sgn;
		}
		else if (fst_coeff < snd_coeff)
		{
			snd_x_trans -= x_diff_sgn;
			snd_y_trans -= y_diff_sgn;
		}*/

		fst.Translate(fst_x_trans, fst_y_trans);
		snd.Translate(snd_x_trans, snd_y_trans);
	}

	else if ((snd_shape == SymEnt::CIRCLE || snd_shape == SymEnt::KHEPERA_ROBOT) && fst_shape == SymEnt::LINE)
	{
		CircularEnt &conv_snd = *dynamic_cast<CircularEnt*>(&snd);

		double proj_diff = conv_snd.GetCenter().GetDistance(proj);
		double x_diff = conv_snd.GetCenter().GetXDiff(proj);
		double y_diff = conv_snd.GetCenter().GetYDiff(proj);


		double x_trans = x_diff / proj_diff * collisionLen;
		double y_trans = y_diff / proj_diff * collisionLen;

		std::cout << collisionLen << "| " << proj_diff << "| " << x_diff << " " << y_diff << "| " << x_trans << " " << y_trans << "\n";

		snd.Translate(x_trans, y_trans);

	}
	else if ((fst_shape == SymEnt::CIRCLE || fst_shape == SymEnt::KHEPERA_ROBOT) && snd_shape == SymEnt::LINE)
		removeCollision(snd, fst, collisionLen, proj);
}


SymEnt* Symulation::GetEntity(uint16_t id)
{
	std::map<uint16_t, SymEnt*>::iterator it = _entities.find(id);

	if (it != _entities.end())
		return _entities[id];
	else
		return NULL;
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

void Symulation::Serialize(std::ofstream& file) const
{
	file.write(reinterpret_cast<const char*>(&_worldWidth), sizeof(_worldWidth));
	file.write(reinterpret_cast<const char*>(&_worldHeight), sizeof(_worldHeight));
	file.write(reinterpret_cast<const char*>(&_time), sizeof(_time)); // do we have to save time to file?
	
	uint16_t size = _entities.size();
	file.write(reinterpret_cast<const char*>(&size), sizeof(size));

	std::map<uint16_t, SymEnt*>::const_iterator it = _entities.begin();

	while (it != _entities.end())
	{
		it->second->Serialize(file);
		it++;
	}
}

void Symulation::Run()
{
	int i = 0;
	while (_isRunning)
	{
		Lock();

		_commMan->SendWorldDescriptionToVisualisers();
		Update(_symulationStep);
		std::cout << "RUNNING: " <<  i++ << std::endl;

		Unlock();
		Sleep(_symulationDelay);
	}

	std::cout << "THREAD END" << std::endl;
}
