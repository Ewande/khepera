#include "Simulation/Simulation.h"

// Simulation object management
extern "C" __declspec(dllexport) Simulation* createSimulation(char* fileName, bool readBinary);
extern "C" __declspec(dllexport) void removeSimulation(Simulation* simulation);
extern "C" __declspec(dllexport) Simulation* cloneSimulation(Simulation* simulation);
extern "C" __declspec(dllexport) void updateSimulation(Simulation* simulation, unsigned int steps);
extern "C" __declspec(dllexport) int getRobotCount(Simulation* simulation);
extern "C" __declspec(dllexport) bool fillRobotsIdArray(Simulation* simulation, int* idArray, int arrLength);

// Robot object management
extern "C" __declspec(dllexport) KheperaRobot* getRobot(Simulation* simulation, int robotId);
extern "C" __declspec(dllexport) int getSensorCount(KheperaRobot* robot);
extern "C" __declspec(dllexport) float getSensorState(KheperaRobot* robot, int sensorNumber);
extern "C" __declspec(dllexport) void setRobotSpeed(KheperaRobot* robot, double leftMotor, double rightMotor);