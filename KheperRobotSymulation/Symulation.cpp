#include "Symulation.h"
#include <iostream>

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
					Point proj;

					double collision_len = it1->second->CollisionLength(*(it2->second), proj);
					if (collision_len >= 0)
					{
						std::cout << "kolizja " << it1->second->GetID() << " i " << it2->second->GetID() << "\n";
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
	if (!(fst_shape % 3) == !(snd_shape % 3)) // TO CHANGE
	{
		CircularEnt &conv_fst = *dynamic_cast<CircularEnt*>(&fst), &conv_snd = *dynamic_cast<CircularEnt*>(&snd);
		Point& center_fst = conv_fst.GetCenter(), center_snd = conv_snd.GetCenter();

		double weights_sum = conv_fst.GetWeight() + conv_snd.GetWeight();
		double fst_coeff = conv_snd.GetWeight() / weights_sum;
		double snd_coeff = conv_fst.GetWeight() / weights_sum;

		double centers_diff = center_fst.GetDistance(center_snd);
		double x_diff = center_fst.GetXDiff(center_snd);
		double y_diff = center_fst.GetYDiff(center_snd);

		double x_diff_sgn = x_diff / abs(x_diff);
		double y_diff_sgn = y_diff / abs(y_diff);

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

		conv_fst.Translate((int) fst_x_trans, (int) fst_y_trans);
		conv_snd.Translate((int) snd_x_trans, (int) snd_y_trans);
	}

	else if (fst_shape != snd_shape && fst_shape == SymEnt::RECTANGLE)
	{
		CircularEnt &conv_snd = *dynamic_cast<CircularEnt*>(&snd);

		double proj_diff = conv_snd.GetCenter().GetDistance(proj);
		double x_diff = conv_snd.GetCenter().GetXDiff(proj);
		double y_diff = conv_snd.GetCenter().GetYDiff(proj);


		double x_trans = x_diff / proj_diff * collisionLen;
		double y_trans = y_diff / proj_diff * collisionLen;

		std::cout << collisionLen << "| " << proj_diff << "| " << x_diff << " " << y_diff << "| " << x_trans << " " << y_trans << "\n";

		snd.Translate((int)x_trans, (int)y_trans);

	}
	else if (fst_shape != snd_shape && snd_shape == SymEnt::RECTANGLE)
		removeCollision(snd, fst, collisionLen, proj);
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

SymEnt* Symulation::GetEntity(uint16_t id)
{
	std::map<uint16_t, SymEnt*>::iterator it = _entities.find(id);

	if (it != _entities.end())
		return _entities[id];
	else
		return NULL;
}

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
		Lock();

		_commMan->SendWorldDescriptionToVisualisers();
		Update(1);
		std::cout << "RUNNING: " <<  i++ << std::endl;

		Unlock();
		Sleep(125);
	}

	std::cout << "THREAD END" << std::endl;
}
