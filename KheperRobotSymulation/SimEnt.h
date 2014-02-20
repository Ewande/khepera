#ifndef SIMENT_H
#define SIMENT_H

#include <stdint.h>

class SimEnt
{
	public:
		// shapes IDs definitions
		const uint8_t RECTANGLE = 0;
		const uint8_t CIRCLE = 1;

		SimEnt(uint16_t id, uint8_t shape, uint32_t weight, bool movable) : _id(id), _shapeID(shape),
			_weight(weight), _movable(movable) {}

		uint16_t GetID() const { return _id; }
		uint8_t GetShapeID() const { return _shapeID; }

		bool IsColliding(const SimEnt& other) { return false; /* TODO: Implement*/ }
		// virtual void Rotate(double angle) = 0; TODO: Later
		virtual void Translate(int x, int y) = 0;
	private:
		const uint16_t   _id;
		const uint8_t    _shapeID;
		const uint32_t   _weight;
		const bool       _movable;
};

#endif