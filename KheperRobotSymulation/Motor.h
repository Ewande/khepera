#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

class Motor
{
	public:
		double GetSpeed() const { return _speed; } // returns speed in [ rad / sec ]
		void SetSpeed(double speed) { _speed = speed; }

	protected:
		double  _speed; // [ rad / sec ]
};

#endif