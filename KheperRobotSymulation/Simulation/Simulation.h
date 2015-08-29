#ifndef SIMULATION_H
#define SIMULATION_H

// SIMULATION CONSTANTS
#define NUMBER_OF_CHECKS            10
#define DIVIDING_LEVEL              3
#define RESERVED_ID_LEVEL           1000
#define NO_COLLISION	            -100
#define INF_COLLISION               1000000

#define DEFAULT_SIMULATION_STEP     0.04
#define DEFAULT_SIMULATION_DELAY    40

#include <map>
#include <iostream>

#include "Entities/SimEnt.h"
#include "Entities/RectangularEnt.h"
#include "Entities/CircularEnt.h"
#include "Entities/KheperaRobot.h"
#include "Entities/LinearEnt.h"
#include "Buffer.h"

class Simulation
{
	public:
        Simulation(unsigned int worldWidth, unsigned int worldHeight, bool addBounds,
                double simulationStep = DEFAULT_SIMULATION_STEP, int simulationDelay = DEFAULT_SIMULATION_DELAY);
        Simulation(std::ifstream& file, double simulationStep = DEFAULT_SIMULATION_STEP,
			int simulationDelay = DEFAULT_SIMULATION_DELAY);
        ~Simulation();

		void addEntity(SimEnt* newEntity);
		void start();
		void update(double deltaTime); // deltaTime in [ sec ]

		void checkCollisions();
		void removeCollision(SimEnt& fst, SimEnt& snd, double collisionLen, Point& proj);

		SimEnt* getEntity(uint16_t id);

		void serialize(Buffer& buffer) const;
		void serialize(std::ofstream& file) const;
	protected:
		std::map<uint16_t, SimEnt*>   _entities;
		uint32_t                      _worldWidth;
		uint32_t                      _worldHeight;
		double                        _time;
		double                        _simulationStep; // in [ s ]
		uint16_t                      _simulationDelay; // in [ ms ]

		bool                          _isRunning;
};


#endif
