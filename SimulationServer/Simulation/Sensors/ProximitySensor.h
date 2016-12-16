#ifndef PROXIMITY_SENSOR_H
#define PROXIMITY_SENSOR_H

#include "Sensor.h"
#include "../Entities/RectangularEnt.h"
#include "../Entities/LinearEnt.h"

class ProximitySensor : public Sensor
{
    public:
        ProximitySensor(double range, float rangeAngle, float placingAngle)
            : Sensor(Sensor::PROXIMITY, range, rangeAngle, placingAngle) {}
        ProximitySensor(std::ifstream& file, bool readBinary) : Sensor(file, readBinary, Sensor::PROXIMITY) {}
        ProximitySensor(const ProximitySensor& other) : Sensor(other) {}
        void updateState(const SimEntMap::const_iterator& firstEntity, 
            const SimEntMap::const_iterator& lastEntity);

    private:
        double detectCircle(CircularEnt& entity, Point& sensor_beg, Point& sensor_end);
        double detectLine(LinearEnt& entity, Point& sensor_beg, Point& sensor_end);
        double detectRectange(RectangularEnt& entity, Point& sensor_beg, Point& sensor_end);
};

#endif