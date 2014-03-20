#ifndef SYMULATION_H
#define SYMULATION_H

#include <map>

#include "SymEnt.h"
#include "KheperaRobot.h"
#include "Buffer.h"
#include "CommunicationManager.h"

class CommunicationManager;

class Symulation
{
	public:
		friend DWORD WINAPI SymulationThreadWrapperFunction(LPVOID threadData);

		Symulation(unsigned int worldWidth, unsigned int worldHeight);
		~Symulation();

		void AddEntity(SymEnt* newEntity);
		void Start(); // starts symulation
		void Update(unsigned int deltaTime); // deltaTime in [ sec ] 

		void SetCommunicationManager(CommunicationManager* commMan) { _commMan = commMan; }

		void Serialize(Buffer& buffer) const;
	private:
		std::map<uint16_t, SymEnt*>   _entities;
		uint32_t                      _worldWidth;
		uint32_t                      _worldHeight;
		uint32_t                      _time;

		bool                          _isRunning;
		CommunicationManager*         _commMan;

		// symulation runs on separete thread
		HANDLE                        _symulationThreadHandle;

		void Run(); // method called from newly created thread for running symulation
};


#endif