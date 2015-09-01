#ifndef ROBOT_H
#define ROBOT_H

#include "CircularEnt.h"
#include <stdint.h>
#include <list>

class Sensor
{
    public:
        Sensor() {}
};

class Motor
{
    public:
        double getSpeed() const { return _speed; } // returns speed in [ rad / sec ]
        void setSpeed(double speed) { _speed = speed; }

    protected:
        double  _speed; // [ rad / sec ]
};

class KheperaRobot : public CircularEnt
{
	public:
		KheperaRobot(uint16_t id, uint32_t weight, double x, double y, double robotRadius, uint16_t wheelRadius,
			uint16_t wheelDistance, float directionAngle = 0);
		KheperaRobot(std::ifstream& file);
        ~KheperaRobot();

		void setRightMotorSpeed(double speed) { _rightMotor.setSpeed(speed); }
		void setLeftMotorSpeed(double speed) { _leftMotor.setSpeed(speed); }

		double getRightMotorSpeed() const { return _rightMotor.getSpeed(); }
		double getLeftMotorSpeed() const { return _leftMotor.getSpeed(); }

		// deltaTime in [ sec ]
		void updatePosition(double deltaTime);

		virtual void serialize(Buffer& buffer);
		virtual void serialize(std::ofstream& file);


	protected:
		uint16_t    _wheelRadius;
		uint16_t    _wheelDistance;

		double       _directionAngle; // angle beetween x axis and robot heading direction, in radians

		Motor       _leftMotor;
		Motor       _rightMotor;

        std::list<Sensor*> _sensors;
};

#endif