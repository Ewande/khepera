#ifndef SENSOR_H
#define SENSOR_H

#include "Entities/KheperaRobot.h"
#include "Entities/LinearEnt.h"
#include "Constants.h"
#include "Math/MathLib.h"

class KheperaRobot;

class Sensor
{
    friend class KheperaRobot;

    public:
        Sensor(double range, float rangeAngle, float placingAngle);
        void placeOnRobot(KheperaRobot* robot) { _robot = robot; }
        void updateState(SimEntMap::const_iterator& firstEntity, SimEntMap::const_iterator& lastEntity);

    private:
        double _range;
        float _rangeAngle;
        KheperaRobot* _robot;
        float _placingAngle;
        float _state;
        int _beams;
};

#endif