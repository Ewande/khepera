#ifndef ROBOT_H
#define ROBOT_H

#include "CircularEnt.h"
#include "Motor.h"

class KheperaRobot : public CircularEnt
{
	public:
		KheperaRobot(uint16_t id, uint32_t weight, double x,
			double y, double robotRadius, uint16_t wheelRadius,
			uint16_t wheelDistance, float directionAngle);
		KheperaRobot(std::ifstream& file);

		void SetRightMotorSpeed(double speed) { _rightMotor.SetSpeed(speed); }
		void SetLeftMotorSpeed(double speed) { _leftMotor.SetSpeed(speed); }

		double GetRightMotorSpeed() const { return _rightMotor.GetSpeed(); }
		double GetLeftMotorSpeed() const { return _leftMotor.GetSpeed(); }

		// deltaTime in [ sec ]
		void UpdatePosition(double deltaTime);

		virtual void Serialize(Buffer& buffer);
		virtual void Serialize(std::ofstream& file);


	protected:
		uint16_t    _wheelRadius;
		uint16_t    _wheelDistance;

		double       _directionAngle; // angle beetween x axis and robot heading direction (don't know if in radians or degrees)

		Motor       _leftMotor;
		Motor       _rightMotor;

		/* TODO: Add sensors */

};

#endif