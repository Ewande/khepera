#include "DistrSimulation.h"
#include <iostream>

DWORD WINAPI SimulationThreadWrapperFunction(LPVOID threadData)
{
    DistrSimulation* sim = static_cast<DistrSimulation*>(threadData);

    sim->run();

    return 0; // unused thread return value
}

DistrSimulation::DistrSimulation(unsigned int worldWidth, unsigned int worldHeight, bool addBounds,
    double simulationStep, int simulationDelay) : Simulation(worldWidth, worldHeight, addBounds, 
    simulationStep, simulationDelay), _commMan(NULL), _simulationThreadHandle(INVALID_HANDLE_VALUE)
{
    InitializeCriticalSection(&_criticalSection);
}

DistrSimulation::DistrSimulation(std::ifstream& file, bool readBinary, double simulationStep,
    int simulationDelay) : Simulation(file, readBinary, simulationStep, simulationDelay),
    _commMan(NULL), _simulationThreadHandle(INVALID_HANDLE_VALUE)
{
    InitializeCriticalSection(&_criticalSection);
}

DistrSimulation::~DistrSimulation()
{
    DeleteCriticalSection(&_criticalSection);
}

void DistrSimulation::start()
{
    Simulation::start();
    _simulationThreadHandle = CreateThread(NULL, 0, SimulationThreadWrapperFunction, 
        static_cast<LPVOID>(this), 0, NULL);
}

void DistrSimulation::run()
{
    std::cout << "SIMULATION STARTED!\n";;
    int i = 0;
    while (_isRunning)
    {
        lock();

        _commMan->sendWorldDescriptionToVisualisers();
        _commMan->sendRobotsStatesToControllers();
        update();
        std::cout << "STEP: " << i++ << "\n";

        unlock();
        Sleep(_simulationDelay);
    }

    std::cout << "SIMULATION ENDED!\n";
}
