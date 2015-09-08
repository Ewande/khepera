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

DistrSimulation::DistrSimulation(std::ifstream& file, double simulationStep,
    int simulationDelay) : Simulation(file, simulationStep, simulationDelay),
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
    _simulationThreadHandle = 
        CreateThread(NULL, 0, SimulationThreadWrapperFunction, static_cast<LPVOID>(this), 0, NULL);
}

void DistrSimulation::run()
{
    int i = 0;
    while (_isRunning)
    {
        lock();

        _commMan->sendWorldDescriptionToVisualisers();
        _commMan->sendRobotsStatesToControllers();
        update(_simulationStep);
        std::cout << "RUNNING: " << i++ << std::endl;

        unlock();
        Sleep(_simulationDelay);
    }

    std::cout << "THREAD END" << std::endl;
}
