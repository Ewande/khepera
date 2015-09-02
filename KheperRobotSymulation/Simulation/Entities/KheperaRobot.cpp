#include "KheperaRobot.h"

KheperaRobot::KheperaRobot(uint16_t id, uint32_t weight, double x,
	double y, double robotRadius, uint16_t wheelRadius, uint16_t wheelDistance,
	float directionAngle) : CircularEnt(id, weight, true, x, y, robotRadius),
	_wheelRadius(wheelRadius), _wheelDistance(wheelDistance), _directionAngle(directionAngle)
{
	_shapeID = SimEnt::KHEPERA_ROBOT;
    setLeftMotorSpeed(0);
    setRightMotorSpeed(0);
}

KheperaRobot::KheperaRobot(std::ifstream& file) : CircularEnt(file)
{
	_shapeID = SimEnt::KHEPERA_ROBOT;

	file.read(reinterpret_cast<char*>(&_wheelRadius), sizeof(_wheelRadius));
	file.read(reinterpret_cast<char*>(&_wheelDistance), sizeof(_wheelDistance));
	file.read(reinterpret_cast<char*>(&_directionAngle), sizeof(_directionAngle));
}

KheperaRobot::~KheperaRobot()
{
    while (!_sensors.empty())
    {
        delete _sensors.back();
        _sensors.pop_back();
    }
}

void KheperaRobot::updatePosition(double deltaTime)
{
	// thanks to http://www.youtube.com/watch?v=aE7RQNhwnPQ 3:30
	// here is more precise equation: http://robotics.stackexchange.com/a/1679

	// angles of which wheels turned during deltaTime
	double leftWheelTurnAngle = _leftMotor.getSpeed() * deltaTime;
	double rightWheelTurnAngle = _rightMotor.getSpeed() * deltaTime;

	double deltaFI = ((_wheelRadius / (float) _wheelDistance) * (rightWheelTurnAngle - leftWheelTurnAngle));
	_directionAngle += deltaFI;

	// _directionAngle is in radians 
	double deltaX = (_wheelRadius / 2.0)*(leftWheelTurnAngle + rightWheelTurnAngle) * cos(_directionAngle);
	double deltaY = (_wheelRadius / 2.0)*(leftWheelTurnAngle + rightWheelTurnAngle) * sin(_directionAngle);

    translate(deltaX, deltaY);
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
			|                                                                              |
			|                                 DIRECTION_ANGLE                              |
			|                                     64 bits                                  |
			|                                                                              |
			|                                                                              |
			+--------------------------------------+---------------------------------------+

DATA_LENGTH = 352 bits

*/

void KheperaRobot::serialize(Buffer& buffer)
{
	CircularEnt::serialize(buffer);

	buffer.pack(htons(_wheelRadius));
	buffer.pack(htons(_wheelDistance));
	buffer.pack(_directionAngle);
    uint16_t sensorCount = _sensors.size();
    buffer.pack(htons(sensorCount));
    for (std::list<Sensor*>::const_iterator it = _sensors.begin(); it != _sensors.end(); it++)
    {
        buffer.pack((*it)->_placingAngle);
        buffer.pack((*it)->_rangeAngle);
        buffer.pack((*it)->_range);
        buffer.pack((*it)->_state);
    }
}

void KheperaRobot::serialize(std::ofstream& file)
{
	CircularEnt::serialize(file);

	file.write(reinterpret_cast<const char*>(&_wheelRadius), sizeof(_wheelRadius));
	file.write(reinterpret_cast<const char*>(&_wheelDistance), sizeof(_wheelDistance));
	file.write(reinterpret_cast<const char*>(&_directionAngle), sizeof(_directionAngle));

	/* TODO: Serialize information about motors(probably about their type) */
}