#ifndef SIMENT_H
#define SIMENT_H

#include <stdint.h>

class SimEnt
{
	public:
		SimEnt(uint16_t id, uint8_t shape);

		uint16_t GetID() const { return _id; }
		uint8_t GetShapeID() const { return _shapeID; }

		bool IsColliding(const SimEnt& other) { return false; /* TODO: Implement*/ }
		void Rotate(double angle) { /* TODO: Implement*/ }
		void Translate(int x, int y) { /* TODO: Implement*/ }
	private:
		const uint16_t _id;
		const uint8_t _shapeID;
};

#endif