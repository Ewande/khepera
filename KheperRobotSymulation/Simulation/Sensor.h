#ifndef SENSOR_H
#define SENSOR_H

#include "Entities/KheperaRobot.h"
#include "Constants.h"
#include "Math/MathLib.h"

class Simulation;
class KheperaRobot;

class Sensor
{
    friend class Simulation;
    friend class KheperaRobot;

    public:
        Sensor(double range, float rangeAngle, float placingAngle);
    private:
        void placeOnRobot(KheperaRobot* robot) { _robot = robot; }
        void updateState(SimEntMap::const_iterator& firstEntity, SimEntMap::const_iterator& lastEntity);

        double _range;
        float _rangeAngle;
        KheperaRobot* _robot;
        float _placingAngle;
        float _state;
};

#endif