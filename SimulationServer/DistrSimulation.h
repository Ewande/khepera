#ifndef DISTR_SIMULATION_H
#define DISTR_SIMULATION_H

#include "CommunicationManager.h"
#include "Simulation/Simulation.h"

class CommunicationManager;

class DistrSimulation : public Simulation
{
public:
    friend class CommunicationManager;
    friend DWORD WINAPI SimulationThreadWrapperFunction(LPVOID threadData);

    DistrSimulation(unsigned int worldWidth, unsigned int worldHeight, bool addBounds,
        double simulationStep = DEFAULT_SIMULATION_STEP, int simulationDelay = DEFAULT_SIMULATION_DELAY);
    DistrSimulation(std::ifstream& file, double simulationStep = DEFAULT_SIMULATION_STEP,
        int simulationDelay = DEFAULT_SIMULATION_DELAY);
    ~DistrSimulation();

    void start(); // starts simulation

    // methods used to lock and unlock Simulation object for only one thread
    // if object is locked, it can't be locked again, until unlocking
    void lock() { (&_criticalSection); }
    void unlock() { (&_criticalSection); }

    void setCommunicationManager(CommunicationManager* commMan) { _commMan = commMan; }
private:
    CommunicationManager*         _commMan;

    // simulation runs in separate thread
    HANDLE                        _simulationThreadHandle;

    // critical section object, used to exclusively lock object for only one thread
    CRITICAL_SECTION             _criticalSection;

    void run(); // method called from newly created thread for running simulation
};


#endif
