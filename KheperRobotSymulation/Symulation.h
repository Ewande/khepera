#ifndef SYMULATION_H
#define SYMULATION_H

#define NUMBER_OF_CHECKS 10

#include <map>

#include "SymEnt.h"
#include "KheperaRobot.h"
#include "Buffer.h"
#include "CommunicationManager.h"

class Symulation
{
	public:
		Symulation(unsigned int worldWidth, unsigned int worldHeight);
		~Symulation();

		void AddEntity(SymEnt* newEntity);
		void Start();
		void Update(unsigned int deltaTime); // deltaTime in [ sec ] 
		void CheckCollisions();
		void removeCollision(SymEnt& fst, SymEnt& snd, double collisionLen, Point& proj);

		void Serialize(Buffer& buffer) const;
	private:
		std::map<uint16_t, SymEnt*>   _entities;
		uint32_t                      _worldWidth;
		uint32_t                      _worldHeight;
		uint32_t                      _time;
};


#endif