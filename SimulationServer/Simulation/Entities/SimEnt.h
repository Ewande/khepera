#ifndef SIMENT_H
#define SIMENT_H

#include <stdint.h>
#include <algorithm>
#include <fstream>

#ifdef _WIN32
#include <Ws2tcpip.h>
#endif

#if defined(__unix__) || defined(__APPLE__)
#include <netinet/in.h>
#endif

#include "../Buffer.h"
#include "../Math/Point.h"
#include "../Math/MathLib.h"

class SimEnt
{
	public:
		// shapes IDs definitions
		static const uint8_t RECTANGLE = 0;
		static const uint8_t CIRCLE = 1;
		static const uint8_t KHEPERA_ROBOT = 2;
		static const uint8_t LINE = 3;

		SimEnt(uint16_t id, uint8_t shape, uint32_t weight, bool movable) : _id(id), _shapeID(shape),
			_weight(weight), _movable(movable) {}
		SimEnt(std::ifstream& file, bool readBinary, uint8_t shapeID);

		virtual ~SimEnt() {}

		int getID() const { return _id; }
        int getShapeID() const { return _shapeID; }
        int getWeight() const { return _weight; }
        bool isMovable() const { return _movable != 0; }

		virtual double collisionLength(SimEnt& other, Point& proj) = 0;
		// virtual void rotate(double angle) = 0; TODO: Later
		virtual void translate(double x, double y) = 0;

        virtual double updatePosition(double deltaTime) { return 0; }

		// serialize for network transmission
		virtual void serialize(Buffer& buffer) = 0;
		// serialize for file storage. WARNING: Uses host-byte-order
		virtual void serialize(std::ofstream& file) = 0;
	protected:

		/* TODO: Maybe we should store color information, so that visualiser user will be able to distinct diffrent entities */
        uint16_t   _id;
        uint8_t    _shapeID;
        uint32_t   _weight;
		uint8_t    _movable; // stored as integer, to be able to send it through socket
};

#endif