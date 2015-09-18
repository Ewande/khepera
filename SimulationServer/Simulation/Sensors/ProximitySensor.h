#ifndef PROXIMITY_SENSOR_H
#define PROXIMITY_SENSOR_H

#include "Sensor.h"

class ProximitySensor : public Sensor
{
    public:
        ProximitySensor(double range, float rangeAngle, float placingAngle)
            : Sensor(Sensor::PROXIMITY, range, rangeAngle, placingAngle) {}
        ProximitySensor(std::ifstream& file, bool readBinary) : Sensor(file, readBinary, Sensor::PROXIMITY) {}
        ProximitySensor(const ProximitySensor& other) : Sensor(other) {}
        void updateState(const SimEntMap::const_iterator& firstEntity, 
            const SimEntMap::const_iterator& lastEntity);
};

#endif