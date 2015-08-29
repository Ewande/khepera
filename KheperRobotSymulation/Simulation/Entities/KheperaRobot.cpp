#include "KheperaRobot.h"

KheperaRobot::KheperaRobot(uint16_t id, uint32_t weight, double x,
	double y, double robotRadius, uint16_t wheelRadius, uint16_t wheelDistance,
	float directionAngle) : CircularEnt(id, weight, true, x, y, robotRadius),
	_wheelRadius(wheelRadius), _wheelDistance(wheelDistance), _directionAngle(directionAngle)
{
	_shapeID = SimEnt::KHEPERA_ROBOT;
}

KheperaRobot::KheperaRobot(std::ifstream& file) : CircularEnt(file)
{
	_shapeID = SimEnt::KHEPERA_ROBOT;

	file.read(reinterpret_cast<char*>(&_wheelRadius), sizeof(_wheelRadius));
	file.read(reinterpret_cast<char*>(&_wheelDistance), sizeof(_wheelDistance));
	file.read(reinterpret_cast<char*>(&_directionAngle), sizeof(_directionAngle));
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

	_center->setCoords(_center->getX() + deltaX, _center->getY() + deltaY); 
	
}

/*
		Serialization format (integers in network-byte-order, doubles and floats in host-byte-order)

			+-------------------+--------------------------------------+-------------------+
			|                   |                                      |                   |
			|   SHAPE_ID        |              ENTITY_ID               |    MOVABLE        |
			|    8 bytes        |               16 bytes               |    8 bytes        |
			+-------------------+--------------------------------------+-------------------+
			|                                                                              |
			|                                WEIGHT                                        |
			|                                32 bytes                                      |
			+------------------------------------------------------------------------------+
			|                                                                              |
			|                                                                              |
			|                                X COORD                                       |
			|                                64 bytes                                      |
			|                                                                              |
			|                                                                              |
			+------------------------------------------------------------------------------+
			|                                                                              |
			|                                                                              |
			|                                Y COORD                                       |
			|                                64 bytes                                      |
			|                                                                              |
			|                                                                              |
			+------------------------------------------------------------------------------+
			|                                                                              |
			|                                                                              |
			|                              ROBOT_RADIUS                                    | 
			|                                64 bytes                                      |
			|                                                                              |
			|                                                                              |
			+--------------------------------------+---------------------------------------+
			|                                      |                                       |
			|             WHEEL_RADIUS             |            WHEEL_DISTANCE             |
			|               16 bytes               |                16 bytes               |
			+--------------------------------------+---------------------------------------+
			|                                                                              |
			|                                                                              |
			|                                 DIRECTION_ANGLE                              |
			|                                     64 bytes                                 |
			|                                                                              |
			|                                                                              |
			+--------------------------------------+---------------------------------------+

DATA_LENGTH = 352 bytes

*/

void KheperaRobot::serialize(Buffer& buffer)
{
	CircularEnt::serialize(buffer);

	buffer.pack(htons(_wheelRadius));
	buffer.pack(htons(_wheelDistance));
	buffer.pack(_directionAngle);
}

void KheperaRobot::serialize(std::ofstream& file)
{
	CircularEnt::serialize(file);

	file.write(reinterpret_cast<const char*>(&_wheelRadius), sizeof(_wheelRadius));
	file.write(reinterpret_cast<const char*>(&_wheelDistance), sizeof(_wheelDistance));
	file.write(reinterpret_cast<const char*>(&_directionAngle), sizeof(_directionAngle));

	/* TODO: Serialize information about motors(probably about their type) */
}