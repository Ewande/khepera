#ifndef SENSOR_H
#define SENSOR_H

#include "Entities/KheperaRobot.h"
#include "Constants.h"

class Simulation;
class KheperaRobot;

class Sensor
{
    friend class Simulation;

    public:
        Sensor(double minRange, double maxRange, float rangeAngle, float placingAngle);
    private:
        void placeOnRobot(KheperaRobot* robot) { _robot = robot; }
        void updateState(SimEntMap::const_iterator& firstEntity, SimEntMap::const_iterator& lastEntity);

        double _minRange;
        double _maxRange;
        float _rangeAngle;
        KheperaRobot* _robot;
        float _placingAngle;
};

#endif