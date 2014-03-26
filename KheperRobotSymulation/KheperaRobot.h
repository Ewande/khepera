#ifndef ROBOT_H
#define ROBOT_H

#define M_PI        3.14159265358979323846

#include "CircularEnt.h"
#include "Motor.h"

class KheperaRobot : public CircularEnt
{
	public:
		KheperaRobot(uint16_t id, uint32_t weight, double x,
			double y, double robotRadius, uint16_t wheelRadius,
			uint16_t wheelDistance, float directionAngle);

		void SetRightMotorSpeed(int16_t speed) { _rightMotor.SetSpeed(speed); }
		void SetLeftMotorSpeed(int16_t speed) { _leftMotor.SetSpeed(speed); }

		int16_t GetRightMotorSpeed() const { return _rightMotor.GetSpeed(); }
		int16_t GetLeftMotorSpeed() const { return _leftMotor.GetSpeed(); }

		// deltaTime in [ sec ]
		void UpdatePosition(unsigned int deltaTime);

		virtual void Serialize(Buffer& buffer);


	protected:
		uint16_t    _wheelRadius;
		uint16_t    _wheelDistance;

		float       _directionAngle; // angle beetween x axis and robot heading direction (don't know if in radians or degrees)

		Motor       _leftMotor;
		Motor       _rightMotor;

		/* TODO: Add sensors */

};

#endif