#ifdef _WIN32
#define DLL_PUBLIC __declspec(dllexport)
#endif

#if defined(__unix__) || defined(__APPLE__)
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#endif

#include "Simulation/Simulation.h"

// Simulation object management
extern "C" DLL_PUBLIC Simulation* createSimulation(char* fileName, bool readBinary);
extern "C" DLL_PUBLIC void removeSimulation(Simulation* simulation);
extern "C" DLL_PUBLIC Simulation* cloneSimulation(Simulation* simulation);
extern "C" DLL_PUBLIC void updateSimulation(Simulation* simulation, int steps);
extern "C" DLL_PUBLIC int getRobotCount(Simulation* simulation);
extern "C" DLL_PUBLIC bool fillRobotsIdArray(Simulation* simulation, int* idArray, int arrLength);

// Robot object management
extern "C" DLL_PUBLIC KheperaRobot* getRobot(Simulation* simulation, int robotId);
extern "C" DLL_PUBLIC int getSensorCount(KheperaRobot* robot);
extern "C" DLL_PUBLIC float getSensorState(KheperaRobot* robot, int sensorNumber);
extern "C" DLL_PUBLIC void setRobotSpeed(KheperaRobot* robot, double leftMotor, double rightMotor);

extern "C" DLL_PUBLIC void moveRandom(Simulation* simulation, KheperaRobot* robot);

// extern "C" DLL_PUBLIC int getXCoord(KheperaRobot* robot);
// extern "C" DLL_PUBLIC int getYCoord(KheperaRobot* robot);