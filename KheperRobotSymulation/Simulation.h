#ifndef SIMULATION_H
#define SIMULATION_H

#define NUMBER_OF_CHECKS 10
#define DIVIDING_LEVEL 3
#define RESERVED_ID_LEVEL 1000
#define NO_COLLISION	-100
#define INF_COLLISION 1000000

#include <map>

#include "SimEnt.h"
#include "RectangularEnt.h"
#include "CircularEnt.h"
#include "KheperaRobot.h"
#include "Buffer.h"
#include "CommunicationManager.h"
#include "LinearEnt.h"

#define DEFAULT_SIMULATION_STEP 0.04
#define DEFAULT_SIMULATION_DELAY 40

class CommunicationManager;

class Simulation
{
	public:
		friend DWORD WINAPI SimulationThreadWrapperFunction(LPVOID threadData);

        Simulation(unsigned int worldWidth, unsigned int worldHeight, bool addBounds);
        Simulation(unsigned int worldWidth, unsigned int worldHeight,
                double simulationStep = DEFAULT_SIMULATION_STEP, int simulationDelay = DEFAULT_SIMULATION_DELAY);
        Simulation(std::ifstream& file, double simulationStep = DEFAULT_SIMULATION_STEP,
			int simulationDelay = DEFAULT_SIMULATION_DELAY);
        ~Simulation();

		void addEntity(SimEnt* newEntity);
		void start(); // starts symulation
		void update(double deltaTime); // deltaTime in [ sec ]

		void checkCollisions();
		void removeCollision(SimEnt& fst, SimEnt& snd, double collisionLen, Point& proj);

		// methods used to lock and unlock Symulation object for only one thread
		// if object is locked, it can't be locked again, until unlocking
		void lock() { (&_criticalSection); }
		void unlock() { (&_criticalSection); }

		void setCommunicationManager(CommunicationManager* commMan) { _commMan = commMan; }

		SimEnt* getEntity(uint16_t id);

		void serialize(Buffer& buffer) const;
		void serialize(std::ofstream& file) const;
	private:
		std::map<uint16_t, SimEnt*>   _entities;
		uint32_t                      _worldWidth;
		uint32_t                      _worldHeight;
		uint32_t                      _time;
		double                        _simulationStep; // in [ s ]
		uint16_t                      _simulationDelay; // in [ ms ]

		bool                          _isRunning;
		CommunicationManager*         _commMan;

		// symulation runs on separete thread
		HANDLE                        _simulationThreadHandle;

		// critical section object, used to exclusively lock object for only one thread
		CRITICAL_SECTION             _criticalSection;

		void run(); // method called from newly created thread for running symulation
};


#endif
