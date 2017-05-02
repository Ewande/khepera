#include "DllInterface.h"

Simulation* createSimulation(char* fileName, bool readBinary)
{
    std::ifstream file(fileName);
    Simulation* simulation = new Simulation(file, readBinary);
    simulation->start();
    return simulation;
}

void removeSimulation(Simulation* simulation)
{
    delete simulation;
}

Simulation* cloneSimulation(Simulation* simulation)
{
    return new Simulation(*simulation);
}

void updateSimulation(Simulation* simulation, int steps)
{
    simulation->update((unsigned int)steps);
}

int getRobotCount(Simulation* simulation)
{
    return simulation->getIdsByShape(SimEnt::KHEPERA_ROBOT).size();
}

bool fillRobotsIdArray(Simulation* simulation, int* idArray, int arrLength)
{
    std::vector<int> robotIds = simulation->getIdsByShape(SimEnt::KHEPERA_ROBOT);
    for (int i = 0; i < arrLength; i++)
        idArray[i] = robotIds[i];
    return robotIds.size() <= (unsigned int)arrLength;
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

float getRobotXCoord(KheperaRobot* robot)
{
    return (float) robot->getCenter().getX();
}

float getRobotYCoord(KheperaRobot* robot)
{
    return (float) robot->getCenter().getY();
}

void teleportRobotRandom(Simulation* simulation, KheperaRobot* robot)
{
    std::uniform_int_distribution<> rand_w(1, simulation->getWorldWidth() - 1);
    std::uniform_int_distribution<> rand_h(1, simulation->getWorldHeight() - 1);
    std::uniform_real_distribution<> rand_angle(0, 6.28);

    int x = rand_w(gen);
    int y = rand_h(gen);
    float angle = (float) rand_angle(gen);
    robot->setDirectionAngle(angle);
    robot->getCenter().setCoords(x, y);
    robot->setLeftMotorSpeed(0);
    robot->setRightMotorSpeed(0);
    simulation->fillDistanceMap();
//     simulation->update();
    x = (int) getRobotXCoord(robot);
    y = (int) getRobotXCoord(robot);

    if (simulation->getNumCollisions() > 0 || x < 0 || x >= simulation->getWorldWidth() 
        || y < 0 || y >= simulation->getWorldHeight())
    {
        teleportRobotRandom(simulation, robot);
    }
}

void setSeed(int seed)
{
    gen.seed(seed);
}