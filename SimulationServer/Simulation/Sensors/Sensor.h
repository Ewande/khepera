#ifndef SENSOR_H
#define SENSOR_H

#include "../Entities/KheperaRobot.h"
#include "../Entities/LinearEnt.h"
#include "../Constants.h"
#include "../Math/MathLib.h"

class KheperaRobot;

class Sensor
{
    friend class KheperaRobot;

    public:
        // sensors types IDs and definitions
        static const uint8_t PROXIMITY = 0;
        static const uint8_t COLOR = 1; // not implemented yet

        Sensor(uint8_t type, double range, float rangeAngle, float placingAngle);
        Sensor(std::ifstream& file, bool readBinary, uint8_t type);
        void placeOnRobot(KheperaRobot* robot) { _robot = robot; }
        virtual void updateState(SimEntMap::const_iterator& firstEntity, SimEntMap::const_iterator& lastEntity) = 0;
        uint8_t getType() { return _type; }

        virtual void serialize(Buffer& buffer) const;
        virtual void serialize(std::ofstream& file) const;

    protected:
        uint8_t _type;
        double _range;
        float _rangeAngle;
        KheperaRobot* _robot;
        float _placingAngle;
        float _state;
        int _beams;
};

#endif