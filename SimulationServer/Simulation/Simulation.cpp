#include "Simulation.h"
#include "Entities/RectangularEnt.h"
#include "Entities/CircularEnt.h"
#include "Entities/KheperaRobot.h"
#include "Entities/LinearEnt.h"
#include "Sensors/ProximitySensor.h"

#include <iterator>

Simulation::Simulation(unsigned int worldWidth, unsigned int worldHeight, bool addBounds,
	double simulationStep , int simulationDelay) :
	_worldWidth(worldWidth), _worldHeight(worldHeight), _simulationStep(simulationStep),
	_simulationDelay(simulationDelay), _isRunning(false)
{
    _hasBounds = addBounds;
    if (_hasBounds)
        this->addBounds();
}

Simulation::Simulation(std::ifstream& file, bool readBinary, double simulationStep, int simulationDelay)
    : _simulationStep(simulationStep), _simulationDelay(simulationDelay), _isRunning(false)
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

	for (int i = 0; i < numberOfEntities; i++)
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
    addEntityInternal(bottom_line);
    addEntityInternal(top_line);
    addEntityInternal(left_line);
    addEntityInternal(right_line);
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
            addEntityInternal(entity);
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
        case SimEnt::LINE:
            newEntity = new LinearEnt(file, readBinary);
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

    for (SimEntMap::iterator it = _entities.begin(); it != _entities.end(); it++)
        delete it->second;
}

void Simulation::addEntity(SimEnt* newEntity)
{
    addEntityInternal(newEntity, RESERVED_ID_LEVEL);
}

void Simulation::addEntityInternal(SimEnt* newEntity, int idLimit)
{
    int new_id = newEntity->getID();
    if (new_id < idLimit)
        _entities[newEntity->getID()] = newEntity;
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

void Simulation::fillDistanceMap()
{
    for (SimEntMap::const_iterator it1 = _entities.begin(); it1 != _entities.end(); it1++)
    {
        int id1 = it1->second->getID();
        for (SimEntMap::const_iterator it2 = std::next(it1); it2 != _entities.end(); it2++)
        {
            int id2 = it2->second->getID();
            Point proj;
            _distances[min(id1, id2) * MAX_ID_LEVEL + max(id1, id2)] =
                -it1->second->collisionLength(*(it2->second), proj);
        }
    }
}

void Simulation::start()
{
	_time = 0;
	_isRunning = true;
    fillDistanceMap();
    updateSensorsState();
}

void Simulation::updateDistanceMap(SimEnt* movingEntity, double distance)
{
    int id1 = movingEntity->getID();
    for (SimEntMap::const_iterator it2 = _entities.begin(); it2 != _entities.end(); it2++)
    {
        int id2 = it2->second->getID();
        _distances[min(id1, id2) * MAX_ID_LEVEL + max(id1, id2)] -= distance;
    }
}


void Simulation::update(double deltaTime)
{
	_time += deltaTime;
    for (SimEntMap::const_iterator it1 = _entities.begin(); it1 != _entities.end(); it1++)
    {
        double moveDistance = it1->second->updatePosition(deltaTime);
        if (moveDistance > 0)
        {
            updateDistanceMap(it1->second, moveDistance);
        }
    }

	checkCollisions();
    updateSensorsState();
}

void Simulation::update(unsigned int steps)
{
    for (unsigned int i = 0; i < steps; i++)
        update(_simulationStep);
}

int Simulation::checkCollisions(bool dryRun)
{
    int num_checks = dryRun ? 1 : NUMBER_OF_CHECKS;
    int num_colls = 0;
    for (int i = 0; i < num_checks; i++)
    {
        for (SimEntMap::const_iterator it1 = _entities.begin(); it1 != _entities.end(); it1++)
        {
            int id1 = it1->second->getID();
            for (SimEntMap::const_iterator it2 = std::next(it1); it2 != _entities.end(); it2++)
            {
                int id2 = it2->second->getID();
                if (_distances[min(id1, id2) * MAX_ID_LEVEL + max(id1, id2)] <= 0)
                {
                    // orthogonal projection onto line (used only when sth is colliding with line)
                    Point proj;

                    double collision_len = it1->second->collisionLength(*(it2->second), proj);
                    _distances[min(id1, id2) * MAX_ID_LEVEL + max(id1, id2)] = -collision_len;

                    if (collision_len > EPS)
                    {
                        num_colls++;
                        if (!dryRun)
                        {
                            removeCollision(*it1->second, *it2->second, collision_len, proj);
                        }
                    }
                }
            }
        }
    }
    return num_colls;
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
		double fst_coeff = snd.getWeight() / weights_sum * fst.isMovable() + fst.getWeight() / weights_sum * (1 - snd.isMovable());
		double snd_coeff = fst.getWeight() / weights_sum * snd.isMovable() + snd.getWeight() / weights_sum * (1 - fst.isMovable());
		double centers_diff = center_fst->getDistance(*center_snd);
        if (centers_diff == 0)
            snd.translate(0.1, 0.1);
        else
        {
		    double x_diff = center_fst->getXDiff(*center_snd);
		    double y_diff = center_fst->getYDiff(*center_snd);

		    double fst_x_trans = x_diff / centers_diff * collisionLen * fst_coeff;
		    double fst_y_trans = y_diff / centers_diff * collisionLen * fst_coeff;

		    double snd_x_trans = (-1) * x_diff / centers_diff * collisionLen * snd_coeff;
		    double snd_y_trans = (-1) * y_diff / centers_diff * collisionLen * snd_coeff;

		    fst.translate(fst_x_trans, fst_y_trans);
            updateDistanceMap(&fst, collisionLen * fst_coeff);
		    snd.translate(snd_x_trans, snd_y_trans);
            updateDistanceMap(&snd, collisionLen * snd_coeff);
        }
	}

	else if ((snd_shape == SimEnt::CIRCLE || snd_shape == SimEnt::KHEPERA_ROBOT) && fst_shape == SimEnt::LINE)
	{
		Point& center = dynamic_cast<CircularEnt*>(&snd)->getCenter();

        double proj_diff = center.getDistance(proj);
        double x_diff = center.getXDiff(proj);
        double y_diff = center.getYDiff(proj);

		if (proj_diff == 0)
			snd.translate(0.1, 0.1);
		else
		{
			double x_trans = x_diff / proj_diff * collisionLen;
			double y_trans = y_diff / proj_diff * collisionLen;
			snd.translate(x_trans, y_trans);
            updateDistanceMap(&snd, collisionLen);

		}
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

std::vector<int> Simulation::getIdsByShape(uint8_t shapeId)
{
    std::vector<int> ids;
    for (SimEntMap::const_iterator it = _entities.begin(); it != _entities.end(); it++)
    {
        if (it->second->getShapeID() == shapeId)
            ids.push_back(it->second->getID());
    }
    return ids;
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
	uint16_t size = (uint16_t) _entities.size();
	file.write(reinterpret_cast<const char*>(&size), sizeof(size));

    for (SimEntMap::const_iterator it = _entities.begin(); it != _entities.end(); it++)
        it->second->serialize(file);
}
