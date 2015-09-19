#include "Simulation.h"
#include "Entities/RectangularEnt.h"
#include "Entities/CircularEnt.h"
#include "Entities/KheperaRobot.h"
#include "Entities/LinearEnt.h"
#include "Sensors/ProximitySensor.h"

Simulation::Simulation(unsigned int worldWidth, unsigned int worldHeight, bool addBounds,
	double simulationStep , int simulationDelay) :
	_worldWidth(worldWidth), _worldHeight(worldHeight), _isRunning(false), _simulationStep(simulationStep),
	_simulationDelay(simulationDelay)
{
    if (_hasBounds = addBounds)
        this->addBounds();
}

Simulation::Simulation(std::ifstream& file, bool readBinary, double simulationStep, int simulationDelay)
    : _isRunning(false), _simulationStep(simulationStep), _simulationDelay(simulationDelay)
{
	uint16_t numberOfEntities;

    if (readBinary)
    {
        file.read(reinterpret_cast<char*>(&_worldWidth), sizeof(_worldWidth));
        file.read(reinterpret_cast<char*>(&_worldHeight), sizeof(_worldHeight));
        file.read(reinterpret_cast<char*>(&_time), sizeof(_time));
        file.read(reinterpret_cast<char*>(&_hasBounds), sizeof(_hasBounds));
        file.read(reinterpret_cast<char*>(&numberOfEntities), sizeof(numberOfEntities));
    }
    else
        file >> _worldWidth >> _worldHeight >> _time >>  _hasBounds >> numberOfEntities;

    if (_hasBounds)
        this->addBounds();

	for (uint16_t i = 0; i < numberOfEntities; i++)
	{
        SimEnt* newEntity = readEntity(file, readBinary);
        if (newEntity != NULL)
        {
            addEntity(newEntity);
            if (newEntity->getShapeID() == SimEnt::KHEPERA_ROBOT)
            {
                uint16_t numberOfSensors;
                if (readBinary)
                    file.read(reinterpret_cast<char*>(&numberOfSensors), sizeof(numberOfSensors));
                else
                    file >> numberOfSensors;
                for (uint16_t i = 0; i < numberOfSensors; i++)
                {
                    Sensor* sensor = readSensor(file, readBinary);
                    if (sensor != NULL)
                        addSensor(sensor, newEntity->getID());
                    else
                        ;// handle exception
                }
            }
        }
        else
            ;// handle exception
	}

}

void Simulation::addBounds()
{
    LinearEnt* bottom_line = new LinearEnt(RESERVED_ID_LEVEL + 1, 0, 0, _worldWidth, 0);
    LinearEnt* top_line = new LinearEnt(RESERVED_ID_LEVEL + 2, 0, _worldHeight, _worldWidth, _worldHeight);
    LinearEnt* left_line = new LinearEnt(RESERVED_ID_LEVEL + 3, 0, 0, 0, _worldHeight);
    LinearEnt* right_line = new LinearEnt(RESERVED_ID_LEVEL + 4, _worldWidth, 0, _worldWidth, _worldHeight);
    _entities[RESERVED_ID_LEVEL + 1] = bottom_line;
    _entities[RESERVED_ID_LEVEL + 2] = top_line;
    _entities[RESERVED_ID_LEVEL + 3] = left_line;
    _entities[RESERVED_ID_LEVEL + 4] = right_line;
}

Simulation::Simulation(const Simulation& other)
{
    _worldWidth = other._worldWidth;
    _worldHeight = other._worldHeight;
    _time = other._time;
    _simulationStep = other._simulationStep;
    _simulationDelay = other._simulationDelay;
    _hasBounds = other._hasBounds;
    _isRunning = other._isRunning;

    for (SimEntMap::const_iterator it = other._entities.begin(); it != other._entities.end(); it++)
    {
        SimEnt* entity;
        switch (it->second->getShapeID())
        {
            case SimEnt::RECTANGLE:
                entity = new RectangularEnt(*dynamic_cast<RectangularEnt*>(it->second));
                break;
            case SimEnt::CIRCLE:
                entity = new CircularEnt(*dynamic_cast<CircularEnt*>(it->second));
                break;
            case SimEnt::KHEPERA_ROBOT:
                entity = new KheperaRobot(*dynamic_cast<KheperaRobot*>(it->second));
                break;
            case SimEnt::LINE:
                entity = new LinearEnt(*dynamic_cast<LinearEnt*>(it->second));
                break;
            default:
                entity = NULL;
                break;
        }
        if (entity != NULL)
            _entities[entity->getID()] = entity;
    }
}

SimEnt* Simulation::readEntity(std::ifstream& file, bool readBinary)
{
    uint8_t shapeID;
    if (readBinary)
        file.read(reinterpret_cast<char*>(&shapeID), sizeof(shapeID));
    else
    {
        uint16_t shapeID16;
        file >> shapeID16;
        shapeID = (uint8_t) shapeID16;
    }
    SimEnt* newEntity;

    switch (shapeID)
    {
        case SimEnt::CIRCLE:
            newEntity = new CircularEnt(file, readBinary);
            break;
        case SimEnt::RECTANGLE:
            newEntity = new RectangularEnt(file, readBinary);
            break;
        case SimEnt::KHEPERA_ROBOT:
            newEntity = new KheperaRobot(file, readBinary);
            break;
        default:
            newEntity = NULL; /* TODO: Exception handling */
            break;
    }

    return newEntity;
}

Sensor* Simulation::readSensor(std::ifstream& file, bool readBinary)
{
    uint8_t type;
    if (readBinary)
        file.read(reinterpret_cast<char*>(&type), sizeof(type));
    else
    {
        uint16_t type16;
        file >> type16;
        type = (uint8_t) type16;
    }
    Sensor* newSensor;

    switch (type)
    {
        case Sensor::PROXIMITY:
            newSensor = new ProximitySensor(file, readBinary);
            break;
        default:
            newSensor = NULL;
            break;
    }
    return newSensor;
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
        return true;
    }
    return false;
}

void Simulation::start()
{
	_time = 0;
	_isRunning = true;
    updateSensorsState();
}

void Simulation::update(double deltaTime)
{
	_time += deltaTime;
    for (SimEntMap::const_iterator it = _entities.begin(); it != _entities.end(); it++)
		it->second->updatePosition(deltaTime);
	checkCollisions();
    updateSensorsState();
}

void Simulation::update(unsigned int steps)
{
    for (unsigned int i = 0; i < steps; i++)
        update(_simulationStep);
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
    for (SimEntMap::const_iterator it = _entities.begin(); it != _entities.end(); it++)
    {
        if (it->second->getShapeID() == SimEnt::KHEPERA_ROBOT)
            dynamic_cast<KheperaRobot*>(it->second)->updateSensorsState(_entities.begin(), _entities.end());
    }
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
    buffer.pack(_hasBounds);
	buffer.pack(htons(static_cast<uint16_t>(_entities.size())));

    for (SimEntMap::const_iterator it = _entities.begin(); it != _entities.end(); it++)
        it->second->serialize(buffer);
}

void Simulation::serialize(std::ofstream& file) const
{
	file.write(reinterpret_cast<const char*>(&_worldWidth), sizeof(_worldWidth));
	file.write(reinterpret_cast<const char*>(&_worldHeight), sizeof(_worldHeight));
	file.write(reinterpret_cast<const char*>(&_time), sizeof(_time)); // do we have to save time to file?
    file.write(reinterpret_cast<const char*>(&_hasBounds), sizeof(_hasBounds));
	uint16_t size = _entities.size();
	file.write(reinterpret_cast<const char*>(&size), sizeof(size));

    for (SimEntMap::const_iterator it = _entities.begin(); it != _entities.end(); it++)
        it->second->serialize(file);
}
