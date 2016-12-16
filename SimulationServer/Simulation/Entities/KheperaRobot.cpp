#include "KheperaRobot.h"
#include "../Sensors/Sensor.h"
#include "../Sensors/ProximitySensor.h"

KheperaRobot::KheperaRobot(uint16_t id, uint32_t weight, double x,
	double y, double robotRadius, uint16_t wheelRadius, uint16_t wheelDistance,
	float directionAngle) : CircularEnt(id, weight, true, x, y, robotRadius),
	_wheelRadius(wheelRadius), _wheelDistance(wheelDistance), _directionAngle(directionAngle)
{
	_shapeID = SimEnt::KHEPERA_ROBOT;
    setLeftMotorSpeed(0);
    setRightMotorSpeed(0);
}

KheperaRobot::KheperaRobot(std::ifstream& file, bool readBinary) : CircularEnt(file, readBinary)
{
	_shapeID = SimEnt::KHEPERA_ROBOT;
    if (readBinary)
    {
        file.read(reinterpret_cast<char*>(&_wheelRadius), sizeof(_wheelRadius));
        file.read(reinterpret_cast<char*>(&_wheelDistance), sizeof(_wheelDistance));
        file.read(reinterpret_cast<char*>(&_directionAngle), sizeof(_directionAngle));
    }
    else
        file >> _wheelRadius >> _wheelDistance >> _directionAngle;
}

KheperaRobot::KheperaRobot(const KheperaRobot& other) : CircularEnt(other)
{
    _wheelRadius = other._wheelRadius;
    _wheelDistance = other._wheelDistance;
    _directionAngle = other._directionAngle;
    _leftMotor = other._leftMotor;
    _rightMotor = other._rightMotor;
    for (std::vector<Sensor*>::const_iterator it = other._sensors.begin(); it != other._sensors.end(); it++)
    {
        Sensor* sensor;
        switch ((*it)->getType())
        {
            case Sensor::PROXIMITY:
                sensor = new ProximitySensor(*dynamic_cast<ProximitySensor*>(*it));
                sensor->placeOnRobot(this);
                break;
            default:
                sensor = NULL;
                break;
        }
        if (sensor != NULL)
            addSensor(sensor);
    }
}

KheperaRobot::~KheperaRobot()
{
    for (std::vector<Sensor*>::iterator sensIt = _sensors.begin(); sensIt != _sensors.end(); sensIt++)
        delete *sensIt;
}

bool KheperaRobot::getSensorState(unsigned int sensorNumber, float& state) const
{
    bool isIndexValid = sensorNumber < _sensors.size();
    if(isIndexValid)
        state = _sensors[sensorNumber]->getState();
    return isIndexValid;
}

double KheperaRobot::updatePosition(double deltaTime)
{
	// thanks to http://www.youtube.com/watch?v=aE7RQNhwnPQ 3:30
	// here is more precise equation: http://robotics.stackexchange.com/a/1679

	// angles of which wheels turned during deltaTime
	double leftWheelTurnAngle = _leftMotor.getSpeed() * deltaTime;
	double rightWheelTurnAngle = _rightMotor.getSpeed() * deltaTime;

	double deltaFI = ((_wheelRadius / (float) _wheelDistance) * (rightWheelTurnAngle - leftWheelTurnAngle));
	_directionAngle += (float) deltaFI;

	// _directionAngle is in radians 
	double deltaX = (_wheelRadius / 2.0) * (leftWheelTurnAngle + rightWheelTurnAngle) * cos(_directionAngle);
	double deltaY = (_wheelRadius / 2.0) * (leftWheelTurnAngle + rightWheelTurnAngle) * sin(_directionAngle);

    translate(deltaX, deltaY);
    return sqrt(deltaX * deltaX + deltaY * deltaY);
}

void KheperaRobot::updateSensorsState(const SimEntMap::const_iterator& firstEntity, 
    const SimEntMap::const_iterator& lastEntity)
{
    for (std::vector<Sensor*>::const_iterator it = _sensors.begin(); it != _sensors.end(); it++)
        (*it)->updateState(firstEntity, lastEntity);
}

void KheperaRobot::addSensor(Sensor* sensor)
{
    _sensors.push_back(sensor);
}

/*
		Serialization format (integers in network-byte-order, doubles and floats in host-byte-order)

			+-------------------+--------------------------------------+-------------------+
			|                   |                                      |                   |
			|   SHAPE_ID        |              ENTITY_ID               |    MOVABLE        |
			|    8 bit s        |               16 bits                |    8 bits         |
			+-------------------+--------------------------------------+-------------------+
			|                                                                              |
			|                                WEIGHT                                        |
			|                                32 bits                                       |
			+------------------------------------------------------------------------------+
			|                                                                              |
			|                                                                              |
			|                                X COORD                                       |
			|                                64 bits                                       |
			|                                                                              |
			|                                                                              |
			+------------------------------------------------------------------------------+
			|                                                                              |
			|                                                                              |
			|                                Y COORD                                       |
			|                                64 bits                                       |
			|                                                                              |
			|                                                                              |
			+------------------------------------------------------------------------------+
			|                                                                              |
			|                                                                              |
			|                              ROBOT_RADIUS                                    | 
			|                                64 bits                                       |
			|                                                                              |
			|                                                                              |
			+--------------------------------------+---------------------------------------+
			|                                      |                                       |
			|             WHEEL_RADIUS             |            WHEEL_DISTANCE             |
			|               16 bits                |                16 bits                |
			+--------------------------------------+---------------------------------------+
			|                                                                              |
			|                                 DIRECTION_ANGLE                              |
			|                                     32 bits                                  |
			+--------------------------------------+---------------------------------------+

DATA_LENGTH = 320 bits

*/

void KheperaRobot::serialize(Buffer& buffer)
{
	CircularEnt::serialize(buffer);

	buffer.pack(htons(_wheelRadius));
	buffer.pack(htons(_wheelDistance));
	buffer.pack(_directionAngle);
    buffer.pack(htons(static_cast<uint16_t>(_sensors.size())));
    for (std::vector<Sensor*>::const_iterator it = _sensors.begin(); it != _sensors.end(); it++)
        (*it)->serialize(buffer);
}

void KheperaRobot::serialize(std::ofstream& file)
{
	CircularEnt::serialize(file);

	file.write(reinterpret_cast<const char*>(&_wheelRadius), sizeof(_wheelRadius));
	file.write(reinterpret_cast<const char*>(&_wheelDistance), sizeof(_wheelDistance));
	file.write(reinterpret_cast<const char*>(&_directionAngle), sizeof(_directionAngle));
    uint16_t numberOfSensors = (uint16_t) _sensors.size();
    file.write(reinterpret_cast<const char*>(&numberOfSensors), sizeof(numberOfSensors));
    for (std::vector<Sensor*>::const_iterator it = _sensors.begin(); it != _sensors.end(); it++)
        (*it)->serialize(file);

	/* TODO: Serialize information about motors(probably about their type) */
}

void KheperaRobot::serializeForController(Buffer& buffer)
{
    buffer.pack(htons(static_cast<uint16_t>(_sensors.size())));
    for (std::vector<Sensor*>::const_iterator it = _sensors.begin(); it != _sensors.end(); it++)
        buffer.pack((*it)->_state);
}