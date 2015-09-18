#include "DllInterface.h"

Simulation* createSimulation(char* fileName, bool readBinary)
{
    std::ifstream file(fileName);
    return new Simulation(file, readBinary);
}

void removeSimulation(Simulation* simulation)
{
    delete simulation;
}

Simulation* cloneSimulation(Simulation* simulation)
{
    return new Simulation(*simulation);
}