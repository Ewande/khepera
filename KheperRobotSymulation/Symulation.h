#ifndef SYMULATION_H
#define SYMULATION_H

#include <map>

#include "SymEnt.h"

class Symulation
{
	public:
		Symulation();
		~Symulation();

		void AddEntity(SymEnt* newEntity);
		void Start();
	private:
		std::map<uint16_t, SymEnt*>   _entities;
		uint32_t                 _time;
};


#endif