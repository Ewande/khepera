#ifndef SIMULATION_H
#define SIMULATION_H

#include <map>
#include <iostream>

#include "Entities/SimEnt.h"
#include "Sensors/Sensor.h"
#include "Buffer.h"
#include "Constants.h"

class Simulation
{
	public:
        Simulation(unsigned int worldWidth, unsigned int worldHeight, bool addBounds,
                double simulationStep = DEFAULT_SIMULATION_STEP, int simulationDelay = DEFAULT_SIMULATION_DELAY);
        Simulation(std::ifstream& file, bool readBinary, double simulationStep = DEFAULT_SIMULATION_STEP,
			int simulationDelay = DEFAULT_SIMULATION_DELAY);
        Simulation(const Simulation& simulation);
        ~Simulation();

		void addEntity(SimEnt* newEntity);
        bool addSensor(Sensor* sensor, uint16_t id);
		void start();
		void update(double deltaTime); // deltaTime in [ s ]
		SimEnt* getEntity(uint16_t id);

		void serialize(Buffer& buffer) const;
		void serialize(std::ofstream& file) const;

	protected:
        void checkCollisions();
        void removeCollision(SimEnt& fst, SimEnt& snd, double collisionLen, Point& proj);
        void updateSensorsState();

		SimEntMap                     _entities;
        std::list<Sensor*>            _sensors;
		uint32_t                      _worldWidth;
		uint32_t                      _worldHeight;
		double                        _time;
		double                        _simulationStep; // in [ s ]
		uint16_t                      _simulationDelay; // in [ ms ]
        bool                          _hasBounds;

		bool                          _isRunning;

    private:
        void addBounds();
        SimEnt* readEntity(std::ifstream& file, bool readBinary);
        Sensor* readSensor(std::ifstream& file, bool readBinary);
};


#endif
