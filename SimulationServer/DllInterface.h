#include "Simulation/Simulation.h"

extern "C" __declspec(dllexport) Simulation* createSimulation(char* fileName, bool readBinary);
extern "C" __declspec(dllexport) void removeSimulation(Simulation* simulation);
extern "C" __declspec(dllexport) Simulation* cloneSimulation(Simulation* simulation);