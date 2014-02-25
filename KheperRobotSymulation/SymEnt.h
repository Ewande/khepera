#ifndef SIMENT_H
#define SIMENT_H

#include <stdint.h>
#include <algorithm>
#include <Ws2tcpip.h>

#include "Buffer.h"

class SymEnt
{
	public:
		// shapes IDs definitions
		static const uint8_t RECTANGLE = 0;
		static const uint8_t CIRCLE = 1;

		SymEnt(uint16_t id, uint8_t shape, uint32_t weight, bool movable) : _id(id), _shapeID(shape),
			_weight(weight), _movable(movable) {}

		uint16_t GetID() const { return _id; }
		uint8_t GetShapeID() const { return _shapeID; }

		bool IsColliding(const SymEnt& other) { return false; /* TODO: Implement*/ }
		// virtual void Rotate(double angle) = 0; TODO: Later
		virtual void Translate(int x, int y) = 0;

		virtual Buffer* Serialize() = 0;
	protected:
		const uint8_t    _shapeID;
		const uint16_t   _id;
		const uint32_t   _weight;
		const uint8_t    _movable; // stored as integer, to be able to send it through socket
};

#endif