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

void updateSimulation(Simulation* simulation, unsigned int steps)
{
    simulation->update(steps);
}

KheperaRobot* getRobot(Simulation* simulation, int robotId)
{
    SimEnt* entity = simulation->getEntity(robotId);
    if (entity != NULL && entity->getShapeID() == SimEnt::KHEPERA_ROBOT)
        return dynamic_cast<KheperaRobot*>(entity);
    else
        return NULL;
}

int getSensorCount(KheperaRobot* robot)
{
    return robot->getSensorCount();
}

float getSensorState(KheperaRobot* robot, int sensorNumber)
{
    float sensorState;
    return robot->getSensorState(sensorNumber, sensorState) ? sensorState : -1;
}

void setRobotSpeed(KheperaRobot* robot, double leftMotor, double rightMotor)
{
    robot->setLeftMotorSpeed(leftMotor);
    robot->setRightMotorSpeed(rightMotor);
}