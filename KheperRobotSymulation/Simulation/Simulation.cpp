#include "Simulation.h"

Simulation::Simulation(unsigned int worldWidth, unsigned int worldHeight, bool addBounds,
	double simulationStep , int simulationDelay) :
	_worldWidth(worldWidth), _worldHeight(worldHeight), _isRunning(false), _simulationStep(simulationStep),
	_simulationDelay(simulationDelay)
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
}

Simulation::Simulation(std::ifstream& file, double simulationStep, int simulationDelay)
    : _isRunning(false), _simulationStep(simulationStep), _simulationDelay(simulationDelay)
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
		SimEnt* newEntity;

		switch (shapeID)
		{
			case SimEnt::CIRCLE: newEntity = new CircularEnt(file); break;
			case SimEnt::RECTANGLE: newEntity = new RectangularEnt(file); break;
			case SimEnt::KHEPERA_ROBOT: newEntity = new KheperaRobot(file); break;

			default: newEntity = NULL; /* TODO: Exception handling */
		}
		addEntity(newEntity);
	}
}

Simulation::~Simulation()
{
	_isRunning = false; // to stop _simulationThreadHandle

	SimEntMap::iterator it = _entities.begin();

	while (it != _entities.end())
	{
		delete it->second;
		it++;
	}

    std::list<Sensor*>::iterator sensIt = _sensors.begin();
    while (sensIt != _sensors.end())
    {
        delete *sensIt;
        sensIt++;
    }
}

void Simulation::addEntity(SimEnt* newEntity)
{
	int new_id = newEntity->getID();
	if (new_id < RESERVED_ID_LEVEL)
		_entities[new_id] = newEntity;
}

bool Simulation::addSensor(Sensor* sensor, uint16_t id)
{
    SimEnt* entity = getEntity(id);
    if (entity && entity->getShapeID() == SimEnt::KHEPERA_ROBOT)
    {
        KheperaRobot* robot = dynamic_cast<KheperaRobot*>(entity);
        sensor->placeOnRobot(robot);
        robot->addSensor(sensor);
        _sensors.push_back(sensor);
        return true;
    }
    return false;
}

void Simulation::start()
{
	_time = 0;
	_isRunning = true;
}

void Simulation::update(double deltaTime)
{
	_time += deltaTime;
    for (SimEntMap::const_iterator it = _entities.begin(); it != _entities.end(); it++)
		it->second->updatePosition(deltaTime);
	checkCollisions();
    updateSensorsState();
}

void Simulation::checkCollisions()
{
	SimEntMap::iterator it1, it2;
	for (int i = 0; i < NUMBER_OF_CHECKS; i++)
		for (it1 = _entities.begin(); it1 != _entities.end(); it1++)
			for (it2 = it1; it2 != _entities.end(); it2++)
			{
				if (it1->second->getID() != it2->second->getID())
				{
					// orthogonal projection onto line (used only when sth is colliding with line)
					Point proj;

					double collision_len = it1->second->collisionLength(*(it2->second), proj);
					if (collision_len > EPS)
					{
						std::cout << "kolizja " << it1->second->getID() << " i " << it2->second->getID() << "\n";
						std::cout << "--- dlugosc: " << collision_len << "\n\n";
						removeCollision(*it1->second, *it2->second, collision_len, proj);
					}
				}
			}
}

void Simulation::removeCollision(SimEnt& fst, SimEnt& snd, double collisionLen, Point& proj)
{
	int fst_shape = fst.getShapeID();
	int snd_shape = snd.getShapeID();
	if (fst_shape != SimEnt::LINE && snd_shape != SimEnt::LINE)
	{
        Point* center_fst;
		Point* center_snd;
		
		if (fst_shape == SimEnt::RECTANGLE)
            center_fst = &dynamic_cast<RectangularEnt*>(&fst)->getCenter();
		else // if it is Circular Entity or Robot
            center_fst = &dynamic_cast<CircularEnt*>(&fst)->getCenter();

        if (snd_shape == SimEnt::RECTANGLE)
            center_snd = &dynamic_cast<RectangularEnt*>(&snd)->getCenter();
        else // if it is Circular Entity or Robot
            center_snd = &dynamic_cast<CircularEnt*>(&snd)->getCenter();

		double weights_sum = fst.getWeight() + snd.getWeight();
		double fst_coeff = snd.getWeight() / weights_sum;
		double snd_coeff = fst.getWeight() / weights_sum;

		double centers_diff = center_fst->getDistance(*center_snd);
		double x_diff = center_fst->getXDiff(*center_snd);
		double y_diff = center_fst->getYDiff(*center_snd);

		double fst_x_trans = x_diff / centers_diff * collisionLen * fst_coeff;
		double fst_y_trans = y_diff / centers_diff * collisionLen * fst_coeff;

		double snd_x_trans = (-1) * x_diff / centers_diff * collisionLen * snd_coeff;
		double snd_y_trans = (-1) * y_diff / centers_diff * collisionLen * snd_coeff;

		fst.translate(fst_x_trans, fst_y_trans);
		snd.translate(snd_x_trans, snd_y_trans);
	}

	else if ((snd_shape == SimEnt::CIRCLE || snd_shape == SimEnt::KHEPERA_ROBOT) && fst_shape == SimEnt::LINE)
	{
		Point& center = dynamic_cast<CircularEnt*>(&snd)->getCenter();

        double proj_diff = center.getDistance(proj);
        double x_diff = center.getXDiff(proj);
        double y_diff = center.getYDiff(proj);


		double x_trans = x_diff / proj_diff * collisionLen;
		double y_trans = y_diff / proj_diff * collisionLen;

		//std::cout << collisionLen << "| " << proj_diff << "| " << x_diff << " " << y_diff << "| " << x_trans << " " << y_trans << "\n";

		snd.translate(x_trans, y_trans);

	}
	else if ((fst_shape == SimEnt::CIRCLE || fst_shape == SimEnt::KHEPERA_ROBOT) && snd_shape == SimEnt::LINE)
		removeCollision(snd, fst, collisionLen, proj);
}

void Simulation::updateSensorsState()
{
    for (std::list<Sensor*>::const_iterator it = _sensors.begin(); it != _sensors.end(); it++)
        (*it)->updateState(_entities.begin(), _entities.end());
}

SimEnt* Simulation::getEntity(uint16_t id)
{
	SimEntMap::iterator it = _entities.find(id);

	if (it != _entities.end())
		return _entities[id];
	else
		return NULL;
}

/*
		Serialization format (all numbers except for time in network byte order - time is in host-byte order)
	+-------------------+--------------------------------------+-------------------+
	|                                                                              |
	|                              WORLD_WIDTH                                     |
	|                                32 bits                                       |
	+------------------------------------------------------------------------------+
	|                                                                              |
	|                              WORLD_HEIGHT                                    |
	|                                32 bits                                       |
	+------------------------------------------------------------------------------+
	|                                                                              |
	|                                  TIME                                        |
	|                                64 bits                                       |
	+-------------------+--------------------------------------+-------------------+
	|                                      |                                       |
	|          NUMBER_OF_ENTITIES          |              ENTITIES_DATA            |
	|               16 bits                |              variable length          |
	+--------------------------------------+---------------------------------------+

*/

void Simulation::serialize(Buffer& buffer) const
{
	buffer.pack(htonl(_worldWidth));
	buffer.pack(htonl(_worldHeight));
	buffer.pack(_time);
	buffer.pack(htons(static_cast<uint16_t>(_entities.size())));

    for (SimEntMap::const_iterator it = _entities.begin(); it != _entities.end(); it++)
        it->second->serialize(buffer);
}

void Simulation::serialize(std::ofstream& file) const
{
	file.write(reinterpret_cast<const char*>(&_worldWidth), sizeof(_worldWidth));
	file.write(reinterpret_cast<const char*>(&_worldHeight), sizeof(_worldHeight));
	file.write(reinterpret_cast<const char*>(&_time), sizeof(_time)); // do we have to save time to file?
	
	uint16_t size = _entities.size();
	file.write(reinterpret_cast<const char*>(&size), sizeof(size));

    for (SimEntMap::const_iterator it = _entities.begin(); it != _entities.end(); it++)
        it->second->serialize(file);
}