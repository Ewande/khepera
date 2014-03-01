#ifndef SYMULATION_H
#define SYMULATION_H

#include <map>

#include "SymEnt.h"
#include "Buffer.h"
#include "CommunicationManager.h"

class Symulation
{
	public:
		Symulation(unsigned int worldWidth, unsigned int worldHeight);
		~Symulation();

		void AddEntity(SymEnt* newEntity);
		void Start();

		void Serialize(Buffer& buffer) const;
	private:
		std::map<uint16_t, SymEnt*>   _entities;
		uint32_t                      _worldWidth;
		uint32_t                      _worldHeight;
		uint32_t                      _time;
};


#endif