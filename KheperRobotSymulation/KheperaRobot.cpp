#include "KheperaRobot.h"

KheperaRobot::KheperaRobot(uint16_t id, uint32_t weight, double x,
	double y, double robotRadius, uint16_t wheelRadius, uint16_t wheelDistance,
	float directionAngle) : CircularEnt(id, weight, true, x, y, robotRadius),
	_wheelRadius(wheelRadius), _wheelDistance(wheelDistance), _directionAngle(directionAngle)
{
	_shapeID = SymEnt::KHEPERA_ROBOT;
}

void KheperaRobot::UpdatePosition(unsigned int deltaTime)
{
	// thanks to http://www.youtube.com/watch?v=aE7RQNhwnPQ 3:30
	// here is more precise equation: http://robotics.stackexchange.com/a/1679

	// angles of which wheels turned during deltaTime
	int leftWheelTurnAngle = _leftMotor.GetSpeed() * deltaTime;
	int rightWheelTurnAngle = _rightMotor.GetSpeed() * deltaTime;

	float deltaFI = ((_wheelRadius / (float) _wheelDistance) * (rightWheelTurnAngle - leftWheelTurnAngle));
	_directionAngle += deltaFI;
	/* FIXME: _directionAngle is probably in radians, but i have to check it, when visualisuator will be able to show robot */
	double deltaX = (_wheelRadius / 2.0)*(leftWheelTurnAngle + rightWheelTurnAngle) * cos(/*M_PI**/_directionAngle);
	double deltaY = (_wheelRadius / 2.0)*(leftWheelTurnAngle + rightWheelTurnAngle) * sin(/*M_PI**/_directionAngle);

	_center->Translate(deltaX, deltaY);
	
}

/*
		Serialization format (all numbers in network byte order)
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
			|                                  DIRECTION_ANGLE                             |
			|                                     32 bytes                                 |
			+--------------------------------------+---------------------------------------+

DATA_LENGTH = 256 bytes

*/

void KheperaRobot::Serialize(Buffer& buffer)
{
	CircularEnt::Serialize(buffer);
	buffer.Pack(htons(_wheelRadius));
	buffer.Pack(htons(_wheelDistance));
	buffer.Pack(_directionAngle);
}