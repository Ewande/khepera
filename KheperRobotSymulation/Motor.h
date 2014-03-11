#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

class Motor
{
	public:
		int16_t GetSpeed() const { return _speed; } // returns speed in [ rad / sec ]
		void SetSpeed(int16_t speed) { _speed = speed; }

	protected:
		int16_t _speed; // [ rad / sec ]
};

#endif