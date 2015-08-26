#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

class Motor
{
	public:
		double getSpeed() const { return _speed; } // returns speed in [ rad / sec ]
		void setSpeed(double speed) { _speed = speed; }

	protected:
		double  _speed; // [ rad / sec ]
};

#endif