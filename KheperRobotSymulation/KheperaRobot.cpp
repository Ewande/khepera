#include "KheperaRobot.h"

KheperaRobot::KheperaRobot(uint16_t id, uint32_t weight, uint32_t x,
	uint32_t y, uint32_t robotRadius, uint16_t wheelRadius, uint16_t wheelDistance,
	int8_t directionAngle) : CircularEnt(id, weight, true, x, y, robotRadius),
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

	int deltaFI = (_wheelRadius / _wheelDistance) * (deltaTime - deltaTime);
	_directionAngle += deltaFI;

	/* FIXME: _directionAngle is probably in radians, but i have to check it, when visualisuator will be able to show robot */
	int deltaX = (_wheelRadius / 2)*(leftWheelTurnAngle + rightWheelTurnAngle) * cos(_directionAngle);
	int deltaY = (_wheelRadius / 2)*(leftWheelTurnAngle + rightWheelTurnAngle) * sin(_directionAngle);

	_x += deltaX;
	_y += deltaY;
	
}